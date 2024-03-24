#pragma once

#define GL_SILENCE_DEPRECATION

#include "../base/iresource-manager.hpp"

#include "../../core/game-object/components/material.hpp"

#include <OpenGL/gl3.h>

#include <map>
#include <string>

using namespace std;

enum glApiTextureTypes {
    glApi_DIFFUSE_TEXTURE,
    glApi_SPECULAR_TEXTURE
};

struct CubeMap {
    unsigned int textureId = 0;
    unsigned int VAO = 0;
};

class ResourceManager: public IResourceManager {
private:
    // Key: path, Value: TextureID
    map<string, unsigned int> textures;

    // Key: path
    map<string, CubeMap> cubeMaps;

    // Key: path, Value: VAO
    map<string, unsigned int> models;

// ===
// Singleton pattern
// ===
public:
    static ResourceManager& getInstance() {
        static ResourceManager instance;

        return instance;
    }
    ResourceManager(ResourceManager const&) = delete;
    void operator=(ResourceManager const&)  = delete;

public:
    ResourceManager();

    // ===
    // Getters
    // ===
    static unsigned int getTextureId(const Material* material, glApiTextureTypes textureType);

    static CubeMap* getCubeMap(const string& path);

public:
    bool isResourceLoaded(ResourceType type, const string& path) override;

    void handleTexture(const string& path, unsigned char* data, TextureFormat format, int height, int width) override;

    void handleCubeMap(const string& path, const vector<unsigned char*>* data, TextureFormat format, int height, int width) override;

    void handleModel(const string& path) override;

};
