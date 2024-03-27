#include "point-light.hpp"

PointLight::PointLight() = default;

PointLight::PointLight(float linear, float quadratic)
    : Light(), linear(linear), quadratic(quadratic)
    {}

PointLight::PointLight(float linear, float quadratic, vec3 ambient, vec3 diffuse, vec3 specular)
    :  linear(linear), quadratic(quadratic), Light(ambient, diffuse, specular)
    {}
