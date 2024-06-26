#include "model-parser.hpp"

#include "../hierarchy.hpp"

#include "material-info.hpp"

ModelParser::ModelParser(Loader *loader, string texturesDirectory):
        loader(loader), texturesDirectory(std::move(texturesDirectory))
{ }

GameObject* ModelParser::parse(const aiScene* scene) {
    auto* wrapper = new GameObject();

    processMaterials(scene, wrapper);

    GameObject* mesh = nodeToGameObject(scene, scene->mRootNode);

    Hierarchy::setParent(mesh, wrapper);

    return wrapper;
}

GameObject* ModelParser::nodeToGameObject(const aiScene* scene, const aiNode* node) {
    auto* result = new GameObject();

    // decomposeNodeTransform(node, *result);

    for(unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        Mesh* meshComponent = processMesh(mesh);

        result->components.add(meshComponent);
    }

    for(unsigned int i = 0; i < node->mNumChildren; i++) {
        GameObject* child = nodeToGameObject(scene, node->mChildren[i]);

        Hierarchy::setParent(child, result);
    }

    return result;
}

void ModelParser::decomposeNodeTransform(const aiNode* node, const GameObject& gameObject) {
    aiVector3t<float> position;
    aiVector3t<float> scale;
    aiQuaterniont<float> rotation;

    node->mTransformation.Decompose(scale,rotation, position);

    gameObject.transform->setPosition(position.x, position.y, position.z);
    gameObject.transform->setRotation(rotation.w, rotation.x, rotation.y, rotation.z);
    gameObject.transform->setScale(scale.x, scale.y, scale.z);
}

Mesh* ModelParser::processMesh(aiMesh* mesh) {
    auto convertToVec3 = [](aiVector3D vec) { return Vec3(vec.x, vec.y, vec.z); };
    auto convertToVec2 = [](aiVector3D vec) { return Vec2(vec.x, vec.y); };

    vector<Vertex> vertices;
    vector<unsigned int> indices;

    // Vertices
    for (int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;

        vertex.position = convertToVec3(mesh->mVertices[i]);
        vertex.normal = convertToVec3(mesh->mNormals[i]);

        if(mesh->HasTextureCoords(0)) {
            vertex.tangent = convertToVec3(mesh->mTangents[i]);
            vertex.texCoords = convertToVec2(mesh->mTextureCoords[0][i]);
        }

        vertices.push_back(vertex);
    }

    // Indices
    for (int i = 0; i < mesh->mNumFaces; i++) {
        const aiFace face = mesh->mFaces[i];

        for (int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    // Material
    const Material* material = materials[static_cast<int>(mesh->mMaterialIndex)];

    return new Mesh(vertices, indices, material);
}

void ModelParser::processMaterials(const aiScene* scene, GameObject* result) {
    for(int i=0; i < scene->mNumMaterials; i++) {
        Material* material = processMaterial(scene->mMaterials[i]);

        materials[i] = material;

        result->addDataItem(material);
    }
}

Material* ModelParser::processMaterial(const aiMaterial* material) const {
    auto materialInfo = MaterialInfo(material);

    auto* result = loader->getCachedMaterial(materialInfo);

    if(result != nullptr)
        return result;

    result = new Material();

    result->Kd = materialInfo.diffuseColor;
    result->Ks = materialInfo.specularColor;

    result->roughness = materialInfo.roughness;
    result->metalness = materialInfo.metalness;

    result->diffuseTexture = loadTextureByType(material, aiTextureType_DIFFUSE);
    result->specularTexture = loadTextureByType(material, aiTextureType_SPECULAR);
    result->normalTexture = loadTextureByType(material, aiTextureType_NORMALS);
    result->roughnessTexture = loadTextureByType(material, aiTextureType_DIFFUSE_ROUGHNESS);
    result->aoTexture = loadTextureByType(material, aiTextureType_AMBIENT);

    loader->cacheMaterial(materialInfo, result);
    materialInfo.print();

    return result;
}

Texture ModelParser::loadTextureByType(const aiMaterial* material, const aiTextureType type) const {
    if(material->GetTextureCount(type) > 0) {
        aiString file;
        material->GetTexture(type, 0, &file);

        const string fileStr = file.C_Str();

        const string filename = fileStr.substr(fileStr.find_last_of("/\\") + 1);
        const string path = texturesDirectory + "/" + filename;

        const Texture* texture = loader->loadTexture(path);

        if(texture != nullptr)
            return *texture;
    }

    return *loadDefaultTexture(type);
}

Texture* ModelParser::loadDefaultTexture(const aiTextureType type) const {
    switch (type) {
        case aiTextureType_NORMALS:
            return loader->loadTexture("textures/default_normal.png", TEXTURE_2D_OPTIONS);
        default:
            return loader->loadTexture("textures/solid_white.png", TEXTURE_2D_OPTIONS);
    }
}