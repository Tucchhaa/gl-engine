#pragma once

#include <vector>

#include "component.hpp"
#include "../../structures/material.hpp"

using namespace std;

class Terrain : public ObjectComponent {
private:
    Texture* terrainTexture;

    int resolution;

public:
    const int VERTICES_PER_PATCH = 4;

    Terrain(Texture* terrainTexture, int resolution);

    vector<float> calculatePatches();

    int getVerticesCount() const;

    Texture* getTexture();
};
