#include "direct-light.hpp"

DirectLight::DirectLight() : Light() {}

DirectLight::DirectLight(vec3 ambient, vec3 diffuse, vec3 specular) : Light(ambient, diffuse, specular)
{}
