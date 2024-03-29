#pragma once

#include "../../structures/texture.hpp"

using namespace std;

struct Material {
public:
    vector<Texture> specularTextures;
    vector<Texture> diffuseTextures;
};
