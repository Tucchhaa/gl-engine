#include "light.hpp"

Light::Light() = default;

Light::Light(vec3 ambient, vec3 diffuse, vec3 specular)
    : ambient(ambient), diffuse(diffuse), specular(specular)
{}
