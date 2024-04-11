#include "transform.hpp"

using namespace std;

const Transform Transform::World = Transform(vec3(0), quat(vec3(0, 0, 0)), vec3(1));

Transform::Transform(vec3 position, quat rotation, vec3 scale) :
    position(position), scale(scale), rotation(rotation)
{ }

void Transform::setValues(vec3 position, quat rotation, vec3 scale) {
    this->position = position;
    this->rotation = rotation;
    this->scale = scale;
}

void Transform::translate(vec3 vector, const Transform* transform) {
    quat rotation = this->rotation;
    
    vector.z = -vector.z;
    
    vec3 displacement = rotation * vector;
    
    position += displacement;
}

void Transform::rotate(quat rotation, const Transform* transform) {
    if(transform != nullptr && transform != this) {
        quat relativeRotation = (transform->rotation * rotation) * conjugate(transform->rotation);
        
        this->rotation = relativeRotation * this->rotation;
    }
    else
        this->rotation *= rotation;
}

void Transform::scaleBy(vec3 scale) {
    // (a.x * b.x, a.y * b.y, a.z * b.z)
    this->scale = this->scale * scale;
}

// ===
// Getters
// ===

mat4 Transform::getTransformMatrix() const {
    return transformMatrix;
}

mat3 Transform::getNormalMatrix() const {
    return normalMatrix;
}

vec3 Transform::getDirectionVector() const {
    return direction;
}

vec3 Transform::getPosition() const {
    return position;
}

vec3 Transform::getScale() const {
    return scale;
}

quat Transform::getRotation() const {
    return rotation;
}

vec3 Transform::getAbsolutePosition() const {
    return parentAbsolutePosition + position;
}

vec3 Transform::getAbsoluteScale() const {
    return parentAbsoluteScale * scale;
}

quat Transform::getAbsoluteRotation() const {
    return parentAbsoluteRotation * rotation;
}

// ===
// Calculations
// ===

void Transform::updateAbsoluteValues(Transform* parentTransform) {
    parentAbsolutePosition = parentTransform->parentAbsolutePosition + parentTransform->position;
    parentAbsoluteRotation = parentTransform->parentAbsoluteRotation * parentTransform->rotation;
    parentAbsoluteScale = parentTransform->parentAbsoluteScale * parentTransform->scale;

    recalculate();
}

void Transform::recalculate() {
    transformMatrix = calculateTransformMatrix();
    normalMatrix = calculateNormalMatrix();
    direction = calculateDirection();
}

mat4 Transform::calculateTransformMatrix() const {
    mat4 matrix = mat4(1.0f);

    matrix = glm::translate(matrix, getAbsolutePosition());
    matrix = matrix * mat4_cast(getAbsoluteRotation());
    matrix = glm::scale(matrix, getAbsoluteScale());

    return matrix;
}

mat3 Transform::calculateNormalMatrix() const {
    mat3 inverseScale(0);

    inverseScale[0][0] = 1/(parentAbsoluteScale.x * scale.x);
    inverseScale[1][1] = 1/(parentAbsoluteScale.y * scale.y);
    inverseScale[2][2] = 1/(parentAbsoluteScale.z * scale.z);

    mat3 matrix = inverseScale * mat3_cast(getAbsoluteRotation());

    return matrix;
}

vec3 Transform::calculateDirection() const {
    return getAbsoluteRotation() * vec3(0, 0, -1);
}