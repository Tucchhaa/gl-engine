#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "component.hpp"

using namespace glm;

class Transform : public ObjectComponent {
public:
    static const Transform World;

private:
    vec3 parentAbsolutePosition = vec3(0, 0, 0);

    vec3 parentAbsoluteScale = vec3(1, 1, 1);

    quat parentAbsoluteRotation = quat(vec3(0, 0, 0));

    // ===

    vec3 position;

    vec3 scale;

    quat rotation;

    // ===

    mat4 transformMatrix = mat4(1.0f);

    mat3 normalMatrix = mat3(1.0f);

    vec3 direction = vec3(0, 0, 1);

public:
    explicit Transform(vec3 position = vec3(0, 0, 0), quat rotation = quat(vec3(0, 0, 0)), vec3 scale = vec3(1, 1, 1));

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

    void recalculate();

    mat4 calculateTransformMatrix() const;

    mat3 calculateNormalMatrix() const;

    vec3 calculateDirection() const;

// ===
// Getters
// ===
public:
    mat4 getTransformMatrix() const;
    
    mat3 getNormalMatrix() const;
    
    vec3 getDirectionVector() const;

    vec3 getPosition() const;

    vec3 getScale() const;

    quat getRotation() const;

    vec3 getAbsolutePosition() const;

    vec3 getAbsoluteScale() const;

    quat getAbsoluteRotation() const;
};
