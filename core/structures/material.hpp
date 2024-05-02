#pragma once

#include "texture.hpp"

using namespace std;

struct Material {
public:
    vector<Texture> specularTextures;
    vector<Texture> diffuseTextures;
    vector<Texture> normalTextures;
    vector<Texture> roughnessTextures;
    vector<Texture> aoTextures;

    Material();

    Material(
        Texture specularTexture, Texture diffuseTexture, Texture normalTexture,
        Texture roughnessTexture, Texture aoTexture
    );
};
