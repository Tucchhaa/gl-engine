#pragma once

#include <vector>

#include "component.hpp"
#include "../../structures/material.hpp"

using namespace std;

class Terrain : public ObjectComponent {
private:
    Texture* terrainTexture;

    int patchResolution;

public:
    Terrain(Texture* terrainTexture, int patchResolution);

    vector<float> calculatePatches();

    int getPatchesNum();

    Texture* getTexture();
};
