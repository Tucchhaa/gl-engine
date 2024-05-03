#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include <queue>
#include <vector>
#include <string>
#include <map>

#include "../apis/base/iresource-manager.hpp"

#include "game-object.hpp"
#include "components/mesh.hpp"

#include "structures/material.hpp"
#include "structures/texture.hpp"

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

private:
    class ModelParser {
    private:
        // MaterialId - Material struct
        map<int, Material> materials;

        Loader* loader;

        const string texturesDirectory;

    public:
        ModelParser(Loader* loader, string texturesDirectory);

        GameObject* parse(const aiScene* scene);

    private:
        GameObject* parseNodeToGameObject(const aiScene* scene, const aiNode* node);

        Mesh* processMesh(const aiScene* scene, aiMesh* mesh);

        Material processMaterial(const aiMaterial* material) const;

        Texture loadTextureByType(const aiMaterial* material, aiTextureType type) const;

        Texture* loadDefaultTexture(aiTextureType type) const;

    };

    class aiMaterialInfo {
    public:
        static void print(const aiMaterial* material);

    private:
        template<typename T>
        static void printProperty(const aiMaterial* material, const string& propertyName, const char* key, unsigned int type,
        unsigned int idx);
    };
};
