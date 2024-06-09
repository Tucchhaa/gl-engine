#include "light.hpp"

Light::Light() = default;

Light::Light(float intensity): intensity(intensity) {
}

Light::Light(vec3 ambient, vec3 diffuse, vec3 specular):
    ambient(ambient), diffuse(diffuse), specular(specular)
{}

Light::Light(float intensity, vec3 ambient, vec3 diffuse, vec3 specular)
    : intensity(intensity), ambient(ambient), diffuse(diffuse), specular(specular)
{}
