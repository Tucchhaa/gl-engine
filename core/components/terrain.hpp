#pragma once

#include <vector>

#include "component.hpp"
#include "../imesh.hpp"
#include "../structures/material.hpp"

using namespace std;

class Terrain : public ObjectComponent, public IMesh {
private:
    Texture* terrainTexture;

    int resolution;

    vector<float> vertices;

public:
    const int VERTICES_PER_PATCH = 4;

    Terrain(Texture* terrainTexture, int resolution);

    vector<float> calculateVertices();

    Texture* getTexture();

// === IMesh implementation ===
public:
    void* data() override;

    int dataSize() override;

    vector<int> shaderAttributes() override;

    bool usesTessellation() override;

    int getVertexCount() override;

    int getVerticesPerPatch() override;
};
