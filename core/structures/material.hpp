#pragma once

#include "texture.hpp"
#include "vectors.hpp"

using namespace std;

struct Material {
public:
    Texture specularTexture;
    Texture diffuseTexture;
    Texture normalTexture;
    Texture roughnessTexture;
    Texture aoTexture;

    /**
     * Ambient color, to account for light that is scattered about the entire scene
     */
    Vec3 Ka = Vec3(1.0f, 1.0f, 1.0f);

    /**
     * Diffuse color
     */
    Vec3 Kd = Vec3(0.3f, 0.3f, 0.3f);

    /**
     * Specular color
     */
    Vec3 Ks = Vec3(1.0f, 1.0f, 1.0f);

    /**
     * Shininess of the material
     */
    float shininess = 32.0f;


    Material();

    Material(
        Texture specularTexture,
        Texture diffuseTexture,
        Texture normalTexture,
        Texture roughnessTexture,
        Texture aoTexture
    );
};
