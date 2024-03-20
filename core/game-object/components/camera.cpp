#include "camera.hpp"

Camera::Camera(float fov, float near, float far)
    : fov(fov), near(near), far(far) {}

void Camera::setScreenSizes(float screenWidth, float screenHeight) {
    aspect = screenWidth / screenHeight;
    
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;
}

mat4 Camera::getViewMatrix() {
    Transform* transform = Hierarchy::getTransform(GameObjectID);

    mat4 result = mat4(1.0f);
    
    result = result * mat4_cast(transform->rotation);
    result = translate(result, -transform->position);
    
    return result;
}

mat4 Camera::getViewProjectionMatrix() {
    mat4 perspectiveMatrix = perspective(fov, aspect, near, far);
    
    mat4 viewMatrix = getViewMatrix();
    
    mat4 viewProjection = perspectiveMatrix * viewMatrix;
    
    return viewProjection;
}
