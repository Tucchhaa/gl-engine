#pragma once

#include <vector>

#include "component.hpp"

using namespace std;

class CubicPatch : public ObjectComponent {
private:

    vector<float> controlPoints;

public:
    float resolution;

public:
    CubicPatch();

    CubicPatch(float resolution, vector<float> controlPoints);

    int getPatchesNum();

    const vector<float>* getControlPoints();
};

