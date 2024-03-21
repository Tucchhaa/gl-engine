#pragma once

#include "light.hpp"

class PointLight : public Light {
public:
    float linear;
    float quadratic;
    
    PointLight();
    
    PointLight(float linear, float quadratic);
    
    PointLight(vec3 ambient, vec3 diffuse, vec3 specular, float linear, float quadratic);
    
public:
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

