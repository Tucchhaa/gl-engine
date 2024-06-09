#pragma once

#include "light.hpp"

class PointLight : public Light {
public:
    const float lightVolumeMinAttenuation = 0.1;

    float linear = 0.22;
    float quadratic = 0.20;

    PointLight();

    PointLight(float intensity);

    PointLight(float linear, float quadratic);
    
    PointLight(float intensity, float linear, float quadratic, vec3 ambient, vec3 diffuse, vec3 specular);

public:
    float getRadius() const;

public:
    static PointLight* I100() {
        return new PointLight(100);
    }

    static PointLight* I1000() {
        return new PointLight(1000);
    }

    static PointLight* D6() {
        return new PointLight(0.7, 1.8);
    }
    
    static PointLight* D20() {
        return new PointLight(0.22, 0.20);
    }

    static PointLight* D50() {
        return new PointLight(0.09, 0.032);
    }

    static PointLight* D100() {
        return new PointLight(0.045, 0.0075);
    }
    
    static PointLight* D3250() {
        return new PointLight(0.0014, 0.000007);
    }
};

