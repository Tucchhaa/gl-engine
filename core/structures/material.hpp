#pragma once

#include "texture.hpp"

using namespace std;

struct Material {
public:
    vector<Texture> specularTextures;
    vector<Texture> diffuseTextures;

    Material();

    Material(Texture specularTexture, Texture diffuseTexture);
};
