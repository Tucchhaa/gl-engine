#pragma once

#include <vector>

#include "component.hpp"
#include "../imesh.hpp"

#include "../structures/material.hpp"

using namespace std;

class CubicPatch : public ObjectComponent, public IMesh {
public:
    const int VERTICES_PER_PATCH = 16;

    vector<float> controlPoints;

    float tessOuterLevel = 10.0f;

    float tessInnerLevel = 10.0f;

    Material material;

public:
    CubicPatch(vector<float> controlPoints, Material material, float tessOuterLevel = 10.0f, float tessInnerLevel = 10.0f);

// === IMesh implementation ===
public:
    void* data() override;

    int dataSize() override;

    vector<int> shaderAttributes() override;

    bool usesTessellation() override;

    int getVertexCount() override;

    int getVerticesPerPatch() override;
};

