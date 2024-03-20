#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../component.hpp"

using namespace glm;

class Light : public ObjectComponent {
public:
    vec3 ambient = vec3(0.1, 0.1, 0.1);
    vec3 diffuse = vec3(1.0, 1.0, 1.0);
    vec3 specular = vec3(1.0, 1.0, 1.0);
    
    Light();
    
    Light(vec3 ambient, vec3 diffuse, vec3 specular);
};
