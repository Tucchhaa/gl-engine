#include "material.hpp"

#include <utility>

Material::Material() = default;

Material::Material(
        Texture specularTexture,
        Texture diffuseTexture,
        Texture normalTexture,
        Texture roughnessTexture,
        Texture aoTexture
    ) :
        specularTexture(std::move(specularTexture)),
        diffuseTexture(std::move(diffuseTexture)),
        normalTexture(std::move(normalTexture)),
        roughnessTexture(std::move(roughnessTexture)),
        aoTexture(std::move(aoTexture))
{

}
