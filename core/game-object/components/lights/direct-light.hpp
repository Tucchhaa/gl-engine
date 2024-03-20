#pragma once

#include "light.hpp"

class DirectLight : public Light {
public:
    DirectLight();
    
    DirectLight(vec3 ambient, vec3 diffuse, vec3 specular);
    
    vec3 getPosition();
};
