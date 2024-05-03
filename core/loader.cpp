#include "loader.hpp"

#include <iostream>
#include <fstream>
#include <filesystem>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <assimp/postprocess.h>

#include "hierarchy.hpp"


const string RESOURCES_PATH = "/Users/tucha/Repositories/gl-engine/resources";

Loader::Loader(IResourceManager* resourceManager) : resourceManager(resourceManager)
    {}

Loader::~Loader() {
    for(const auto& it: textures) {
        free(it.second);
    }
}

// === Loader functions ===

/**
 * Load texture from the file with default options
 * @param file relative path to the texture
 * @return pointer to the loaded texture, nullptr if the file does not exist
 */
Texture* Loader::loadTexture(const string &file) {
    return loadTexture(file, TEXTURE_2D_OPTIONS);
}

/**
 * Load texture from the file
 * @param file relative path to the texture
 * @param options texture options
 * @return pointer to the loaded texture, nullptr if the file does not exist
 */
Texture* Loader::loadTexture(const string &file, const TextureOptions options) {
    auto cachedTexture = textures.find(file);
    const string fullPath = RESOURCES_PATH + "/" + file;

    if(cachedTexture != textures.end())
        return cachedTexture->second;

    if(isFileExists(fullPath) == false) {
        return nullptr;
    }

    stbi_set_flip_vertically_on_load(true);

    int height, width, nrChannels;
    unsigned char* data = stbi_load(fullPath.c_str(), &width, &height, &nrChannels, 0);

    TextureFormat format = getTextureFormat(nrChannels);

    auto* texture = new Texture(fullPath, width, height, format, data, options);

    resourceManager->handleTexture(texture);
    textures[file] = texture;

    stbi_image_free(data);
    stbi_set_flip_vertically_on_load(false);

    return texture;
}

Texture* Loader::loadCubeMap(const string &directoryPath) {
    auto cached = textures.find(directoryPath);

    if(cached != textures.end())
        return cached->second;

    string fullPath = RESOURCES_PATH + "/" + directoryPath;

    vector<string> textures_faces = {
        "right.jpg", "left.jpg", "top.jpg", "bottom.jpg", "front.jpg", "back.jpg"
    };

    const int N = 6;
    vector<unsigned char*> data(N);

    int width, height, nrChannels;

    for(int i=0; i < N; i++) {
        data[i] = stbi_load((fullPath + "/" + textures_faces[i]).c_str(), &width, &height, &nrChannels, 0);
    }

    TextureFormat format = getTextureFormat(nrChannels);

    auto* texture = new Texture(directoryPath, width, height, format, &data, CUBE_MAP_TEXTURE_OPTIONS);

    resourceManager->handleCubeMap(texture);
    textures[directoryPath] = texture;

    for(int i=0; i < N; i++) {
        stbi_image_free(data[i]);
    }

    return texture;
}

GameObject *Loader::loadModel(const string &file) {
    const string directory = file.substr(0, file.find_last_of('/'));

    return loadModel(file, directory);
}

GameObject *Loader::loadModel(const string &file, const string& texturesDirectory) {
    const aiScene* scene = loadScene(file);

    ModelParser parser(this, texturesDirectory);

    GameObject* result = parser.parse(scene);

    return result;
}

const aiScene* Loader::loadScene(const string &path) {
    const string fullPath = RESOURCES_PATH + "/" + path;

    if(auto it = models.find(path); it != models.end()) {
        return it->second;
    }

    unsigned int flags = aiProcess_RemoveRedundantMaterials | aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals | aiProcess_CalcTangentSpace;
    const aiScene* scene = importer.ReadFile(fullPath, flags);

    if(!scene || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) || !scene->mRootNode) {
        throw std::runtime_error("Couldn't load model");
    }

    models[path] = scene;

    return scene;
}

// ===
// Loader private methods
// ===

TextureFormat Loader::getTextureFormat(const int nrChannels) {
    if (nrChannels == 1)
        return TEXTURE_FORMAT_R;
    if (nrChannels == 3)
        return TEXTURE_FORMAT_RGB;

    return TEXTURE_FORMAT_RGBA;
}

bool Loader::isFileExists(const string& path) {
    std::ifstream file(path);
    return file.good();
}

// === Model Loader ===

Loader::ModelParser::ModelParser(Loader *loader, string texturesDirectory):
        loader(loader), texturesDirectory(std::move(texturesDirectory))
    {}

GameObject* Loader::ModelParser::parse(const aiScene* scene) {
    for(int i=0; i < scene->mNumMaterials; i++) {
        aiMaterial* _material = scene->mMaterials[i];

        Material material = processMaterial(_material);

        materials[i] = material;
    }

    GameObject* result = parseNodeToGameObject(scene, scene->mRootNode);

    return result;
}

GameObject* Loader::ModelParser::parseNodeToGameObject(const aiScene* scene, const aiNode* node) {

    auto* result = Hierarchy::createGameObject();

    for(unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        Mesh* meshComponent = processMesh(scene, mesh);

        Hierarchy::addComponent(result, meshComponent);
    }

    for(unsigned int i = 0; i < node->mNumChildren; i++) {
        GameObject* child = parseNodeToGameObject(scene, node->mChildren[i]);

        Hierarchy::setParent(result, child);
    }

    return result;
}

Mesh* Loader::ModelParser::processMesh(const aiScene* scene, aiMesh* mesh) {
    auto convertToVec3 = [](aiVector3D vec) { return Vec3(vec.x, vec.y, vec.z); };
    auto convertToVec2 = [](aiVector3D vec) { return Vec2(vec.x, vec.y); };

    vector<Vertex> vertices;
    vector<unsigned int> indices;

    // Vertices
    for (int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;

        vertex.position = convertToVec3(mesh->mVertices[i]);
        vertex.normal = convertToVec3(mesh->mNormals[i]);
        vertex.tangent = convertToVec3(mesh->mTangents[i]);

        if (mesh->mTextureCoords[0])
            vertex.texCoords = convertToVec2(mesh->mTextureCoords[0][i]);
        else
            vertex.texCoords = Vec2(0, 0);

        vertices.push_back(vertex);
    }

    // Indices
    for (int i = 0; i < mesh->mNumFaces; i++) {
        const aiFace face = mesh->mFaces[i];

        for (int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    // Material
    const Material material = materials[static_cast<int>(mesh->mMaterialIndex)];

    return new Mesh(vertices, indices, material);
}

Material Loader::ModelParser::processMaterial(const aiMaterial* material) const {
    Material result;

    aiMaterialInfo::print(material);

    result.diffuseTexture = loadTextureByType(material, aiTextureType_DIFFUSE);
    result.specularTexture = loadTextureByType(material, aiTextureType_SPECULAR);
    result.normalTexture = loadTextureByType(material, aiTextureType_HEIGHT);
    result.roughnessTexture = loadTextureByType(material, aiTextureType_DIFFUSE_ROUGHNESS);
    result.aoTexture = loadTextureByType(material, aiTextureType_AMBIENT);

    return result;
}

Texture Loader::ModelParser::loadTextureByType(const aiMaterial* material, const aiTextureType type) const {
    if(material->GetTextureCount(type) > 0) {
        aiString file;
        material->GetTexture(type, 0, &file);

        const string fileStr = file.C_Str();

        const string filename = fileStr.substr(fileStr.find_last_of("/\\") + 1);
        const string path = texturesDirectory + "/" + filename;

        Texture* texture = loader->loadTexture(path);

        if(texture != nullptr)
            return *texture;
    }

    return *loadDefaultTexture(type);
}

Texture* Loader::ModelParser::loadDefaultTexture(const aiTextureType type) const {
    switch (type) {
        case aiTextureType_DIFFUSE:
            return loader->loadTexture("textures/default_diffuse.png", TEXTURE_2D_OPTIONS);
        case aiTextureType_SPECULAR:
            return loader->loadTexture("textures/default_specular.png", TEXTURE_2D_OPTIONS);
        case aiTextureType_HEIGHT:
            return loader->loadTexture("textures/default_normal.png", TEXTURE_2D_OPTIONS);
        // TODO: add default roughness and ao texture
        case aiTextureType_DIFFUSE_ROUGHNESS:
            return loader->loadTexture("textures/default_roughness.png", TEXTURE_2D_OPTIONS);
        case aiTextureType_AMBIENT:
            return loader->loadTexture("textures/default_ao.png", TEXTURE_2D_OPTIONS);
        default:
            throw runtime_error("Can not load default texture");
    }
}

// === aiMaterial Info ===

void Loader::aiMaterialInfo::print(const aiMaterial* material) {
    std::cout << "= Material Info =" << std::endl;

    // output properties
    printProperty<aiString>(material, "Name", AI_MATKEY_NAME);
    printProperty<aiColor4D>(material, "Diffuse color", AI_MATKEY_COLOR_DIFFUSE);
    printProperty<aiColor4D>(material, "Specular color", AI_MATKEY_COLOR_SPECULAR);
    printProperty<aiColor4D>(material, "Ambient color", AI_MATKEY_COLOR_AMBIENT);
    printProperty<aiColor4D>(material, "Emissive color", AI_MATKEY_COLOR_EMISSIVE);
    printProperty<aiColor4D>(material, "Reflective color", AI_MATKEY_COLOR_REFLECTIVE);

    // output textures
    map<aiTextureType, string> typesName = {
        { aiTextureType_NONE, "NONE" },
        { aiTextureType_DIFFUSE, "DIFFUSE" },
        { aiTextureType_SPECULAR, "SPECULAR" },
        { aiTextureType_AMBIENT, "AMBIENT" },
        { aiTextureType_EMISSIVE, "EMISSIVE" },
        { aiTextureType_HEIGHT, "HEIGHT" },
        { aiTextureType_NORMALS, "NORMALS" },
        { aiTextureType_SHININESS, "SHININESS" },
        { aiTextureType_OPACITY, "OPACITY" },
        { aiTextureType_DISPLACEMENT, "DISPLACEMENT" },
        { aiTextureType_LIGHTMAP, "LIGHTMAP" },
        { aiTextureType_REFLECTION, "REFLECTION" },
        { aiTextureType_BASE_COLOR, "BASE_COLOR" },
        { aiTextureType_NORMAL_CAMERA, "NORMAL_CAMERA" },
        { aiTextureType_EMISSION_COLOR, "EMISSION_COLOR" },
        { aiTextureType_METALNESS, "METALNESS" },
        { aiTextureType_DIFFUSE_ROUGHNESS, "DIFFUSE_ROUGHNESS" },
        { aiTextureType_AMBIENT_OCCLUSION, "AMBIENT_OCCLUSION" },
        { aiTextureType_UNKNOWN, "UNKNOWN" },
        { aiTextureType_TRANSMISSION, "TRANSMISSION" }
    };

    for(int type=aiTextureType_NONE; type <= aiTextureType_TRANSMISSION; type++) {
        aiTextureType aiType = static_cast<aiTextureType>(type);
        unsigned int count = material->GetTextureCount(aiType);

        if(count > 0) {
            string typeName = typesName[aiType];
            std::cout << "Texture type: " << typeName << ", count: " << count << ": ";

            for(unsigned int i = 0; i < count; ++i) {
                aiString path;

                if(material->GetTexture(aiType, i, &path) == AI_SUCCESS) {
                    std::cout << path.C_Str() << ", ";
                }
            }

            std::cout << std::endl;
        }
    }
}

template<typename T>
void Loader::aiMaterialInfo::printProperty(
    const aiMaterial* material, const string &propertyName, const char* key,
    unsigned int type, unsigned int idx
) {
    const bool isString = typeid(T) == typeid(aiString);
    const bool isColor  = typeid(T) == typeid(aiColor4D);

    cout << "Property: " << propertyName << ": ";
    if(aiString str; isString && material->Get(key, type, idx, str) == AI_SUCCESS) {
        std::cout << str.C_Str();
    }
    else if(aiColor4D color; isColor && material->Get(key, type, idx, color) == AI_SUCCESS) {
        std::cout << color.r << " " << color.g << " " << color.b;
    }
    else {
        std::cout << "Can not get property";
    }

    cout << std::endl;
}

