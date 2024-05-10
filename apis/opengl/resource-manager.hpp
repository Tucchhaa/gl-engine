#pragma once

#include <map>
#include <string>

#include "../base/iresource-manager.hpp"

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

public:
    ResourceManager();

// ===
// Getters
// ===
public:
    unsigned int getTextureId(Texture* texture);

    unsigned int getTextureId(const Material* material, TextureType textureType);

    CubeMap* getCubeMap(const Texture* texture);

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
