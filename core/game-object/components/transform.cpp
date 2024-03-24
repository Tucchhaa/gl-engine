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

mat4 Transform::getModelMatrix() {
    mat4 matrix = mat4(1.0f);
    
    matrix = glm::translate(matrix, position);
    matrix = matrix * mat4_cast(rotation);
    matrix = glm::scale(matrix, scale);
    
    return matrix;
}

mat3 Transform::getNormalMatrix() {
    mat3 inverseScale(0);
    
    inverseScale[0][0] = 1/scale.x;
    inverseScale[1][1] = 1/scale.y;
    inverseScale[2][2] = 1/scale.z;
    
    mat3 matrix = inverseScale * mat3_cast(rotation);
    
    return matrix;
}

vec3 Transform::getDirectionVector() {
    vec3 direction = vec3(0, 0, 1);
    
    vec3 rotatedDirection = rotation * direction;
    
    // TODO: maybe don't need normalization here
    return normalize(rotatedDirection);
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
