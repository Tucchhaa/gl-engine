#pragma once

#include <map>

#include "loader.hpp"

#include "../components/mesh.hpp"

class ModelParser {
private:
    /**
     * MaterialId - Material struct
     */
    std::map<int, Material*> materials;

    Loader* loader;

    const string texturesDirectory;

public:
    ModelParser(Loader* loader, string texturesDirectory);

    GameObject* parse(const aiScene* scene);

private:
    GameObject* nodeToGameObject(const aiScene* scene, const aiNode* node);

    static void decomposeNodeTransform(const aiNode* node, const GameObject& gameObject);

    Mesh* processMesh(aiMesh* mesh);

    void processMaterials(const aiScene* scene, GameObject* result);

    Material* processMaterial(const aiMaterial* material) const;

    Texture loadTextureByType(const aiMaterial* material, aiTextureType type) const;

    Texture* loadDefaultTexture(aiTextureType type) const;
};
