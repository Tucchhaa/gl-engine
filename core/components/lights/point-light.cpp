#include "point-light.hpp"

PointLight::PointLight() = default;

PointLight::PointLight(float linear, float quadratic)
    : Light(), linear(linear), quadratic(quadratic)
    {}

PointLight::PointLight(float linear, float quadratic, vec3 ambient, vec3 diffuse, vec3 specular)
    :  linear(linear), quadratic(quadratic), Light(ambient, diffuse, specular)
    {}

// TODO: cache the radius
float PointLight::getRadius() const {
    return sqrt(1 / lightVolumeMinAttenuation);

    // const float a = quadratic;
    // const float b = linear;
    // const float c = 1 - 1 / lightVolumeMinAttenuation;
    //
    // const float delta = b*b - 4*a*c;
    //
    // const float radius = (-b + sqrt(delta)) / (2*a);
    //
    // return radius;
}
