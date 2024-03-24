#pragma once

#include <vector>

enum ResourceType {
    RESOURCE_TEXTURE,
    RESOURCE_MODEL,
    RESOURCE_CUBE_MAP,
};

enum TextureFormat {
    TEXTURE_FORMAT_R,
    TEXTURE_FORMAT_RGB,
    TEXTURE_FORMAT_RGBA,
};

using namespace std;

class IResourceManager {
public:
    virtual ~IResourceManager() = default;

    virtual bool isResourceLoaded(ResourceType type, const string& path) = 0;

    virtual void handleTexture(const string& path, unsigned char* data, TextureFormat format, int height, int width) = 0;

    virtual void handleCubeMap(const string& path, const vector<unsigned char*>* data, TextureFormat format, int height, int width) = 0;

    virtual void handleModel(const string& path) = 0;
};
