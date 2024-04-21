#pragma once

#include "point-light.hpp"

const float DEFAULT_CONE_ANGLE = 0.20;
const float DEFAULT_OUTER_ANGLE = 0.06;

class SpotLight : public PointLight {
public:
    /**
     * Cone angle in radians
     */
    float coneAngle = DEFAULT_CONE_ANGLE;

    float outerAngle = DEFAULT_OUTER_ANGLE;

    SpotLight();

    SpotLight(float coneAngle, float edgeAngle, float linear, float quadratic);

private:
    SpotLight(float coneAngle, float edgeAngle, PointLight* pointLight);

public:
    static SpotLight* D6(float coneAngle = DEFAULT_CONE_ANGLE, float edgeAngle = DEFAULT_OUTER_ANGLE) {
        return new SpotLight(coneAngle, edgeAngle, PointLight::D6());
    }

    static SpotLight* D20(float coneAngle = DEFAULT_CONE_ANGLE, float edgeAngle = DEFAULT_OUTER_ANGLE) {
        return new SpotLight(coneAngle, edgeAngle, PointLight::D20());
    }

    static SpotLight* D50(float coneAngle = DEFAULT_CONE_ANGLE, float edgeAngle = DEFAULT_OUTER_ANGLE) {
        return new SpotLight(coneAngle, edgeAngle, PointLight::D50());
    }

    static SpotLight* D100(float coneAngle = DEFAULT_CONE_ANGLE, float edgeAngle = DEFAULT_OUTER_ANGLE) {
        return new SpotLight(coneAngle, edgeAngle, PointLight::D100());
    }

    static SpotLight* D3250(float coneAngle = DEFAULT_CONE_ANGLE, float edgeAngle = DEFAULT_OUTER_ANGLE) {
        return new SpotLight(coneAngle, edgeAngle, PointLight::D3250());
    }
};