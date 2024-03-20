#pragma once

#define GL_SILENCE_DEPRECATION

#include <OpenGL/gl3.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <queue>
#include <vector>
#include <string>
#include <map>

#include "../game-object/components/mesh.hpp"

using namespace std;

class Loader {
private:
    map<string, unsigned int> cachedTextures;

private:
    
    Mesh processMesh(aiMesh* mesh, const aiScene* scene, const string& directory);
    
    Material processMaterial(aiMaterial* material, const string& directory);
    
    vector<unsigned int> loadTexturesByType(aiMaterial* material, aiTextureType type, const string& directory);
    
    unsigned int loadTexture(const char* path);
    
public:
    Loader();
    
    vector<Mesh> loadModel(const string &path);
};
