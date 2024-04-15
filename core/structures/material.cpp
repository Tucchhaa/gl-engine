#include "material.hpp"

Material::Material() = default;

Material::Material(Texture specularTexture, Texture diffuseTexture, Texture normalTexture) {
    specularTextures = { specularTexture };
    diffuseTextures = { diffuseTexture };
    normalTextures = { normalTexture };
}

bool Material::isEmpty() const {
    return specularTextures.empty() && diffuseTextures.empty();
}
