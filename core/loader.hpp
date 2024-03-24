#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <queue>
#include <vector>
#include <string>
#include <map>
#include <utility>

#include "hierarchy.hpp"
#include "game-object/game-object.hpp"

#include "../apis/base/iresource-manager.hpp"

using namespace std;

class CoreLoader {
private:
    IResourceManager* resourceManager;

public:
    CoreLoader(IResourceManager* resourceManager);

    GameObject* loadModel(const char* path);

    void loadTexture(const char* path);

    void loadCubeMap(const char* path);

private:
    static TextureFormat getTextureFormat(int nrChannels);

private:
    class ModelParser {
    private:
        // MaterialId - Material struct
        map<int, Material> materials;

        CoreLoader* loader;

        const string directory;

    public:
        ModelParser(CoreLoader* loader, string directoryPath);

        GameObject* parse(const aiScene* scene);

    private:
        GameObject* parseNodeToGameObject(const aiScene* scene, aiNode* node);

        Mesh* processMesh(const aiScene* scene, aiMesh* mesh);

        Material processMaterial(aiMaterial* material);

        vector<Texture> loadTexturesByType(aiMaterial* material, aiTextureType type);
    };
};
