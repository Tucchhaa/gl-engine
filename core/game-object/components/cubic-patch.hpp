#pragma once

#include <vector>

#include "component.hpp"

#include "../../structures/material.hpp"

using namespace std;

class CubicPatch : public ObjectComponent {
public:
    const int VERTICES_PER_PATCH = 16;

    vector<float> controlPoints;

    float tessOuterLevel = 10.0f;

    float tessInnerLevel = 10.0f;

    Material material;

public:
    CubicPatch(vector<float> controlPoints, Material material, float tessOuterLevel = 10.0f, float tessInnerLevel = 10.0f);

    int getVerticesCount() const;
};

