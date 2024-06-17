#pragma once

#include "texture.hpp"
#include "vectors.hpp"

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
    Vec3 Kd = Vec3(1.0f, 1.0f, 1.0f);

    /**
     * Specular color
     */
    Vec3 Ks = Vec3(0.0f, 0.0f, 0.0f);

    /**
     * Shininess of the material
     */
    float shininess = 32.0f;

    float metalness = 1.;

    float roughness = 1.;

    Material();

    Material(
        Texture specularTexture,
        Texture diffuseTexture,
        Texture normalTexture,
        Texture roughnessTexture,
        Texture aoTexture
    );
};
