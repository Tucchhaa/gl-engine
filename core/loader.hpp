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

    GameObject* loadModel(const char* path);

    const aiScene* loadScene(const string &path);

    Texture* loadTexture(const char* path);
    Texture* loadTexture(const char* path, TextureOptions options);

    Texture* loadCubeMap(const char* path);

private:
    static TextureFormat getTextureFormat(int nrChannels);

    map<string, Texture*> textures;
    map<string, const aiScene*> models;

private:
    class ModelParser {
    private:
        // MaterialId - Material struct
        map<int, Material> materials;

        Loader* loader;

        const string directory;

    public:
        ModelParser(Loader* loader, string directoryPath);

        GameObject* parse(const aiScene* scene);

    private:
        GameObject* parseNodeToGameObject(const aiScene* scene, const aiNode* node);

        Mesh* processMesh(const aiScene* scene, aiMesh* mesh);

        Material processMaterial(const aiMaterial* material) const;

        Texture loadTextureByType(const aiMaterial* material, aiTextureType type) const;

        static string getDefaultTexturePath(aiTextureType type);

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
