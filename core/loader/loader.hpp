#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include <queue>
#include <string>
#include <map>

#include "../../apis/base/iresource-manager.hpp"

#include "../game-object.hpp"
#include "../structures/texture.hpp"

using namespace std;

class Loader {
private:
    string resourcesPath;

    IResourceManager* resourceManager;

    /**
     * When destroyed, all resources will be freed
     */
    Assimp::Importer importer;

    map<string, Texture*> textures;

    map<string, const aiScene*> models;

public:
    explicit Loader();

    ~Loader();

    GameObject* loadModel(const string &file);
    GameObject* loadModel(const string &file, const string& texturesDirectory);

    Texture* loadTexture(const string &file);
    Texture* loadTexture(const string &file, TextureOptions options);

    Texture* loadCubeMap(const string &directoryPath);

private:
    const aiScene* loadScene(const string &path);

    static TextureFormat getTextureFormat(int nrChannels);

    static bool isFileExists(const string& path);
};
