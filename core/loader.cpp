#include "loader.hpp"

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

Texture* Loader::loadTexture(const char* path) {
    return loadTexture(path, TEXTURE_2D_OPTIONS);
}

Texture* Loader::loadTexture(const char *path, const TextureOptions options) {
    auto cached = textures.find(path);

    if(cached != textures.end())
        return cached->second;

    string fullPath = RESOURCES_PATH + "/" + path;

    stbi_set_flip_vertically_on_load(true);

    int height, width, nrChannels;
    unsigned char* data = stbi_load(fullPath.c_str(), &width, &height, &nrChannels, 0);

    TextureFormat format = getTextureFormat(nrChannels);

    auto* texture = new Texture(fullPath, width, height, format, data, options);

    resourceManager->handleTexture(texture);
    textures[path] = texture;

    stbi_image_free(data);
    stbi_set_flip_vertically_on_load(false);

    return texture;
}

Texture* Loader::loadCubeMap(const char *path) {
    auto cached = textures.find(path);

    if(cached != textures.end())
        return cached->second;

    string fullPath = RESOURCES_PATH + "/" + path;

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

    auto* texture = new Texture(path, width, height, format, &data, CUBE_MAP_TEXTURE_OPTIONS);

    resourceManager->handleCubeMap(texture);
    textures[path] = texture;

    for(int i=0; i < N; i++) {
        stbi_image_free(data[i]);
    }

    return texture;
}

GameObject *Loader::loadModel(const char *path) {
    const string strPath(path);
    const string directory = strPath.substr(0, strPath.find_last_of('/'));

    const aiScene* scene = loadScene(strPath);

    ModelParser parser(this, directory);

    GameObject* result = parser.parse(scene);

    return result;
}

const aiScene* Loader::loadScene(const string &path) {
    const string fullPath = RESOURCES_PATH + "/" + path;

    if(auto it = models.find(path); it != models.end()) {
        return it->second;
    }

    const aiScene* scene = importer.ReadFile(fullPath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals | aiProcess_CalcTangentSpace);

    if(!scene || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) || !scene->mRootNode) {
        throw std::runtime_error("Couldn't load model");
    }

    models[path] = scene;

    return scene;
}

// === Model Loader ===
Loader::ModelParser::ModelParser(Loader *loader, string directory):
        loader(loader), directory(std::move(directory))
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

    result.diffuseTextures = loadTexturesByType(material, aiTextureType_DIFFUSE);
    result.specularTextures = loadTexturesByType(material, aiTextureType_SPECULAR);
    result.normalTextures = loadTexturesByType(material, aiTextureType_HEIGHT);

    return result;
}

vector<Texture> Loader::ModelParser::loadTexturesByType(const aiMaterial* material, const aiTextureType type) const {
    vector<Texture> textures;

    for(int i = 0; i < material->GetTextureCount(type); i++)
    {
        aiString file;
        material->GetTexture(type, i, &file);

        string path = directory + "/" + file.C_Str();

        Texture texture = *loader->loadTexture(path.c_str());

        textures.emplace_back(texture);
    }

    if(textures.empty()) {
        string path = getDefaultTexturePath(type);
        Texture texture = *loader->loadTexture(path.c_str());

        textures.emplace_back(texture);
    }

    return textures;
}

string Loader::ModelParser::getDefaultTexturePath(const aiTextureType type) {
    switch (type) {
        case aiTextureType_DIFFUSE:
            return "textures/default_diffuse.jpeg";
        case aiTextureType_SPECULAR:
            return "textures/default_specular.jpeg";
        case aiTextureType_HEIGHT:
            return "textures/default_normal.jpeg";
        default:
            throw runtime_error("Can not load default texture");
    }
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



