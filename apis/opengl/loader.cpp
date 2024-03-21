#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "loader.hpp"

vec3 convertVec3(aiVector3D vec);
unsigned int getTextureFormat(int nrChannels);

const string RESOURCES_PATH = "/Users/tucha/Repositories/gl-engine/gl-engine/resources";
const string MODELS_PATH = RESOURCES_PATH + "/models";

Loader::Loader() {
}

vector<Mesh> Loader::loadModel(const string &path) {
    string directory = path.substr(0, path.find_last_of('/'));

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(MODELS_PATH + "/" + path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);

    if(!scene || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) || !scene->mRootNode) {
        throw std::runtime_error("Couldn't load model");
    }
    
    vector<Mesh> meshes;
    queue<aiNode*> nodes;
    nodes.push(scene->mRootNode);
    
    while(nodes.size() > 0) {
        aiNode* node = nodes.front();
        nodes.pop();
        
        for(unsigned int meshId = 0; meshId < node->mNumMeshes; meshId++) {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[meshId]];
            meshes.push_back(processMesh(mesh, scene, directory));
        }
        
        for(unsigned int childId = 0; childId < node->mNumChildren; childId++) {
            nodes.push(node->mChildren[childId]);
        }
    }
    
    return meshes;
}

Mesh Loader::processMesh(aiMesh* mesh, const aiScene* scene, const string& directory) {
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    Material material;
    
    // Vertices
    for(int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;

        vertex.position = convertVec3(mesh->mVertices[i]);
        vertex.normal = convertVec3(mesh->mNormals[i]);
        
        if(mesh->mTextureCoords[0])
            vertex.texCoords = convertVec3(mesh->mTextureCoords[0][i]);
        else
            vertex.texCoords = vec2(0, 0);
        
        vertices.push_back(vertex);
    }
    
    // Indices
    for(int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        
        for(int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    
    // Material
    if(mesh->mMaterialIndex >= 0) {
        material = processMaterial(scene->mMaterials[mesh->mMaterialIndex], directory);
    }
    
    return Mesh(vertices, indices, material);
}

Material Loader::processMaterial(aiMaterial* material, const string& directory) {
    Material result;
    
    result.diffuseTextures = loadTexturesByType(material, aiTextureType_DIFFUSE, directory);
    result.specularTextures = loadTexturesByType(material, aiTextureType_SPECULAR, directory);
    
    return result;
}

vector<unsigned int> Loader::loadTexturesByType(aiMaterial* material, aiTextureType type, const string& directory) {
    vector<unsigned int> textures;
    
    for(int i = 0; i < material->GetTextureCount(type); i++)
    {
        aiString file;
        material->GetTexture(type, i, &file);
        
        string path = MODELS_PATH + "/" + directory + "/" + file.C_Str();
        map<string, unsigned>::iterator cachedTexture = cachedTextures.find(path);
        
        if(cachedTexture != cachedTextures.end()) {
            textures.push_back(cachedTexture->second);
        }
        else {
            unsigned int textureId = loadTexture(path.c_str());
            
            textures.push_back(textureId);
            
            cachedTextures[path] = textureId;
        }
    }
    
    return textures;
}

unsigned int Loader::loadTexture(const char* path) {
    stbi_set_flip_vertically_on_load(true);
    
    unsigned int textureId;

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    
    int width, height, nrChannels;
    
    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
    unsigned int format = getTextureFormat(nrChannels);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    
    stbi_image_free(data);
    stbi_set_flip_vertically_on_load(false);
    
    return textureId;
}

vec3 convertVec3(aiVector3D vec) {
    return vec3(vec.x, vec.y, vec.z);
}

unsigned int getTextureFormat(int nrChannels) {
    if (nrChannels == 1)
        return GL_RED;
    else if (nrChannels == 3)
        return GL_RGB;
    
//    else if (nrComponents == 4)
    return GL_RGBA;
}
