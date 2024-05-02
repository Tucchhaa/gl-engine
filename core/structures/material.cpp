#include "material.hpp"

Material::Material() = default;

Material::Material(
    Texture specularTexture, Texture diffuseTexture, Texture normalTexture,
    Texture roughnessTexture, Texture aoTexture
) {
    specularTextures = { specularTexture };
    diffuseTextures = { diffuseTexture };
    normalTextures = { normalTexture };
    roughnessTextures = { roughnessTexture };
    aoTextures = { aoTexture };
}
