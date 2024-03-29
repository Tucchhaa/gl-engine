#pragma once

#include <vector>

#include "component.hpp"
#include "material.hpp"

using namespace std;

class Terrain : public ObjectComponent {
private:
    Texture* terrainTexture;

    int patchResolution;

public:
    Terrain(Texture* terrainTexture, int patchResolution);

private:
    vector<float> calculateVertices();
};
