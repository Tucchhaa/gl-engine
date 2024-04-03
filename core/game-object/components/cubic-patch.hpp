#pragma once

#include <vector>

#include "component.hpp"

#include "../../structures/material.hpp"

using namespace std;

class CubicPatch : public ObjectComponent {
private:

    vector<float> controlPoints;

public:
    float resolution;

    Material material;

public:
    CubicPatch(Material material);

    CubicPatch(float resolution, vector<float> controlPoints, Material material);

    int getPatchesNum();

    const vector<float>* getControlPoints();
};

