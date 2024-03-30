#pragma once

#define GL_SILENCE_DEPRECATION

#include <OpenGL/gl3.h>

#include <map>
#include <string>

#include "../base/iresource-manager.hpp"

#include "../../core/game-object/components/material.hpp"

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
    // Key: Loader.TextureID, Value: GL.TextureID
    map<int, unsigned int> textures;

    // Key: Loader.TextureID,
    map<int, CubeMap> cubeMaps;

    // Key: Loader.TextureID,, Value: VAO
    map<int, unsigned int> models;

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
public:
    static unsigned int getTextureId(Texture* texture);

    static unsigned int getTextureId(const Material* material, glApiTextureTypes textureType);

    static CubeMap* getCubeMap(const Texture* texture);

// ===
// Overrode methods
// ===
public:

    void handleTexture(const Texture* texture) override;

    void handleCubeMap(const Texture* texture) override;

    void handleModel(const string& path) override;

// ===
// Converters
// ===
private:
    static int convertTextureWrap(TextureWrap wrap);

    static int convertTextureFilter(TextureFilter filter);

    static int convertTextureFormat(TextureFormat format);
};
