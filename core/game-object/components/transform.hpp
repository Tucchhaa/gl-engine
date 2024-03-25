#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "component.hpp"

using namespace glm;

class Transform : public ObjectComponent {
public:
    static const Transform* World;

private:
    mat4 modelMatrix;

    mat3 normalMatrix;

    vec3 absolutePosition = vec3(0, 0, 0);

    vec3 absoluteScale = vec3(1, 1, 1);

    quat absoluteRotation = quat(vec3(0, 0, 0));

public:
    vec3 position;

    vec3 scale;

    quat rotation;
    
    Transform(vec3 position = vec3(0, 0, 0), quat rotation = quat(vec3(0, 0, 0)), vec3 scale = vec3(1, 1, 1));

public:
    void setValues(vec3 position = vec3(0, 0, 0), quat rotation = quat(vec3(0, 0, 0)), vec3 scale = vec3(1, 1, 1));

    void translate(vec3 vector, const Transform* transform = nullptr);

    void rotate(quat rotation, const Transform* transform = nullptr);

    void scaleBy(vec3 scale);

// ===
// Computations
// ===
public:
    void updateAbsoluteValues(Transform* parentTransform);

    void calculateMatrices();

    mat4 calculateModelMatrix();

    mat3 calculateNormalMatrix();

// ===
// Getters
// ===
public:
    mat4 getModelMatrix() const;
    
    mat3 getNormalMatrix() const;
    
    vec3 getDirectionVector() const;
};
