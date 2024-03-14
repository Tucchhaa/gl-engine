//
//  light.cpp
//  opengl-test
//
//  Created by Eldar Iusupzhanov on 2024/3/7.
//

#include "light.hpp"

Light::Light() {}

Light::Light(vec3 ambient, vec3 diffuse, vec3 specular)
    : ambient(ambient), diffuse(diffuse), specular(specular)
{}
