#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <queue>
#include <vector>
#include <string>
#include <map>
#include <utility>

#include "../apis/base/iresource-manager.hpp"
#include "hierarchy.hpp"

#include "game-object/game-object.hpp"
#include "game-object/components/mesh.hpp"

#include "structures/material.hpp"
#include "structures/texture.hpp"

using namespace std;

class Loader {
private:
    IResourceManager* resourceManager;

public:
    explicit Loader(IResourceManager* resourceManager);

    ~Loader();

    GameObject* loadModel(const char* path);

    Texture* loadTexture(const char* path);
    Texture* loadTexture(const char* path, TextureOptions options);

    Texture* loadCubeMap(const char* path);

private:
    static TextureFormat getTextureFormat(int nrChannels);

    map<string, Texture*> textures;

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
        GameObject* parseNodeToGameObject(const aiScene* scene, aiNode* node);

        Mesh* processMesh(const aiScene* scene, aiMesh* mesh);

        Material processMaterial(aiMaterial* material);

        vector<Texture> loadTexturesByType(aiMaterial* material, aiTextureType type);
    };
};
