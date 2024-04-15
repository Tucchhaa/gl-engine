#pragma once

#include "texture.hpp"

using namespace std;

struct Material {
public:
    vector<Texture> specularTextures;
    vector<Texture> diffuseTextures;
    vector<Texture> normalTextures;

    Material();

    Material(Texture specularTexture, Texture diffuseTexture, Texture normalTexture);

    bool isEmpty() const;
};
