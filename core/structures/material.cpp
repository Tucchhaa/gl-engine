#include "material.hpp"

Material::Material() = default;

Material::Material(Texture specularTexture, Texture diffuseTexture) {
    specularTextures = { specularTexture };
    diffuseTextures = { diffuseTexture };
}
