#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include <queue>
#include <string>
#include <map>

#include "../../apis/base/iresource-manager.hpp"

#include "../components/mesh.hpp"

#include "../structures/texture.hpp"

using namespace std;

class Loader {
private:
    IResourceManager* resourceManager;

    /**
     * When destroyed, all resources will be freed
     */
    Assimp::Importer importer;

public:
    explicit Loader(IResourceManager* resourceManager);

    ~Loader();

    GameObject* loadModel(const string &file);
    GameObject* loadModel(const string &file, const string& texturesDirectory);

    const aiScene* loadScene(const string &path);

    Texture* loadTexture(const string &file);
    Texture* loadTexture(const string &file, TextureOptions options);

    Texture* loadCubeMap(const string &directoryPath);

private:
    static TextureFormat getTextureFormat(int nrChannels);

    static bool isFileExists(const string& path);

    map<string, Texture*> textures;
    map<string, const aiScene*> models;
};
