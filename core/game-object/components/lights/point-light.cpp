#include "point-light.hpp"

PointLight::PointLight(float linear, float quadratic)
    : Light(), linear(linear), quadratic(quadratic)
    {}

PointLight::PointLight(vec3 ambient, vec3 diffuse, vec3 specular, float linear, float quadratic) 
    : Light(ambient, diffuse, specular), linear(linear), quadratic(quadratic)
    {}
