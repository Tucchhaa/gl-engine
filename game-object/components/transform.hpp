#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "component.hpp"

using namespace glm;

class Transform : public ObjectComponent {
public:
    static const Transform* World;
    
public:
    vec3 position;
    vec3 scale;
    quat rotation;
    
    Transform(vec3 position = vec3(0, 0, 0), vec3 scale = vec3(1, 1, 1), quat rotation = quat(vec3(0, 0, 0)));
    
    mat4 getModelMatrix();
    
    mat3 getNormalMatrix();
    
    vec3 getDirectionVector();
    
    void translate(vec3 vector, const Transform* transform = nullptr);
    
    void rotate(quat rotation, const Transform* transform = nullptr);
    
    void scaleBy(vec3 scalation);
};
