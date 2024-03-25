#include "transform.hpp"

using namespace std;

const Transform* Transform::World = new Transform(vec3(0), quat(vec3(0, 0, 0)), vec3(1));


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
    
    vec3 displacement = vector * rotation;
    
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

void Transform::scaleBy(vec3 scalation) {

}

// ===
// Getters
// ===
mat4 Transform::getModelMatrix() const {
    return modelMatrix;
}

mat3 Transform::getNormalMatrix() const {
    return normalMatrix;
}

// TODO: precalculate this like model and normal matrices
vec3 Transform::getDirectionVector() const {
    vec3 direction = vec3(0, 0, 1);

    vec3 rotatedDirection = rotation * direction;

    // TODO: maybe don't need normalization here
    return normalize(rotatedDirection);
}

// ===
// Calculations
// ===

void Transform::updateAbsoluteValues(Transform* parentTransform) {
    absolutePosition = parentTransform->absolutePosition + parentTransform->position;
    absoluteRotation = parentTransform->absoluteRotation * parentTransform->rotation;
    absoluteScale = parentTransform->absoluteScale * parentTransform->scale;

    calculateMatrices();
}

void Transform::calculateMatrices() {
    modelMatrix = calculateModelMatrix();
    normalMatrix = calculateNormalMatrix();
}

mat4 Transform::calculateModelMatrix() {
    mat4 matrix = mat4(1.0f);

    matrix = glm::translate(matrix, absolutePosition + position);
    matrix = matrix * mat4_cast(absoluteRotation * rotation);
    matrix = glm::scale(matrix, absoluteScale * scale);

    return matrix;
}

mat3 Transform::calculateNormalMatrix() {
    mat3 inverseScale(0);

    inverseScale[0][0] = 1/(absoluteScale.x * scale.x);
    inverseScale[1][1] = 1/(absoluteScale.y * scale.y);
    inverseScale[2][2] = 1/(absoluteScale.z * scale.z);

    mat3 matrix = inverseScale * mat3_cast(absoluteRotation * rotation);

    return matrix;
}
