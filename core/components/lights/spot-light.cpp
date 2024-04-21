#include "spot-light.hpp"

SpotLight::SpotLight() = default;

SpotLight::SpotLight(float coneAngle, float edgeAngle, float linear, float quadratic):
        coneAngle(coneAngle), outerAngle(edgeAngle), PointLight(linear, quadratic) {}

SpotLight::SpotLight(float coneAngle, float edgeAngle, PointLight* pointLight):
    SpotLight(coneAngle, edgeAngle, pointLight->linear, pointLight->quadratic) {}
