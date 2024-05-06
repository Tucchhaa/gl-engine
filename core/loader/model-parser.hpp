#pragma once

#include <map>

#include "loader.hpp"

#include "../game-object.hpp"

class ModelParser {
private:
    /**
     * MaterialId - Material struct
     */
    std::map<int, Material> materials;

    Loader* loader;

    const string texturesDirectory;

public:
    ModelParser(Loader* loader, string texturesDirectory);

    GameObject* parse(const aiScene* scene);

private:
    GameObject* parseNodeToGameObject(const aiScene* scene, const aiNode* node);

    static void decomposeNodeTransform(const aiNode* node, const GameObject& gameObject);

    Mesh* processMesh(const aiScene* scene, aiMesh* mesh);

    Material processMaterial(const aiMaterial* material) const;

    Texture loadTextureByType(const aiMaterial* material, aiTextureType type) const;

    Texture* loadDefaultTexture(aiTextureType type) const;
};
