#include "loader.hpp"

#include <iostream>
#include <fstream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <assimp/postprocess.h>

#include "model-parser.hpp"

#include "../../apis/base/iengine.hpp"

Loader::Loader() {
    resourcesPath = IEngine::RESOURCES_PATH;
    resourceManager = IEngine::ResourceManager;
}

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
    const auto cachedTexture = textures.find(file);
    const string fullPath = resourcesPath + "/" + file;

    if(cachedTexture != textures.end())
        return cachedTexture->second;

    if(isFileExists(fullPath) == false) {
        cout << "WARNING: File " << fullPath << " does not exist\n";
        return nullptr;
    }

    stbi_set_flip_vertically_on_load(true);

    int height, width, nrChannels;
    unsigned char* data = stbi_load(fullPath.c_str(), &width, &height, &nrChannels, 0);

    const TextureFormat format = getTextureFormat(nrChannels);

    auto* texture = new Texture(fullPath, width, height, format, data, options);

    resourceManager->handleTexture(texture);
    textures[file] = texture;

    stbi_image_free(data);
    stbi_set_flip_vertically_on_load(false);

    return texture;
}

Texture* Loader::loadCubeMap(const string &directoryPath) {
    const auto cached = textures.find(directoryPath);

    if(cached != textures.end())
        return cached->second;

    const string fullPath = resourcesPath + "/" + directoryPath;

    const vector<string> textures_faces = {
        "right.jpg", "left.jpg", "top.jpg", "bottom.jpg", "front.jpg", "back.jpg"
    };

    constexpr int N = 6;
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

Material* Loader::getCachedMaterial(const MaterialInfo& info) {
    const auto result = materials.find(info);

    if(result == materials.end())
        return nullptr;

    return result->second;
}

void Loader::cacheMaterial(const MaterialInfo &info, Material* material) {
    materials[info] = material;
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
    const string fullPath = resourcesPath + "/" + path;

    if(const auto it = models.find(path); it != models.end()) {
        return it->second;
    }

    constexpr unsigned int flags =
        aiProcess_PreTransformVertices | aiProcess_RemoveRedundantMaterials |
        aiProcess_Triangulate | aiProcess_CalcTangentSpace;

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
    const std::ifstream file(path);

    return file.good();
}
