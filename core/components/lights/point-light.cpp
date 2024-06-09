#include "point-light.hpp"

PointLight::PointLight() = default;

PointLight::PointLight(float intensity): Light(intensity) { }

PointLight::PointLight(float linear, float quadratic)
    : Light(), linear(linear), quadratic(quadratic)
    {}

PointLight::PointLight(float intensity, float linear, float quadratic, vec3 ambient, vec3 diffuse, vec3 specular)
    :  Light(intensity, ambient, diffuse, specular), linear(linear), quadratic(quadratic)
    {}

// TODO: cache the radius
float PointLight::getRadius() const {
    float maxColor = max(max(diffuse.r, diffuse.g), diffuse.b);

    return sqrt(maxColor * intensity / lightVolumeMinAttenuation);

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
