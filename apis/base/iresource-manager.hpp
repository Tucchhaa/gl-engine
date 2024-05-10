#pragma once

#include "../../core/structures/material.hpp"

enum ResourceType {
    RESOURCE_TEXTURE,
    RESOURCE_MODEL,
    RESOURCE_CUBE_MAP,
};

class IResourceManager {
public:
    virtual ~IResourceManager() = default;

    virtual void handleTexture(const Texture* texture) = 0;

    virtual void handleCubeMap(const Texture* texture) = 0;

    virtual void handleModel(const string& path) = 0;
};
