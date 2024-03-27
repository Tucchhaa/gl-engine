#include "camera.hpp"

Camera::Camera(float fov, float near, float far)
    : fov(fov), near(near), far(far) {}

void Camera::setScreenSizes(float screenWidth, float screenHeight) {
    aspect = screenWidth / screenHeight;
    
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;
}

mat4 Camera::getViewMatrix(bool translationEnabled) {
    Transform* transform = Hierarchy::getTransform(GameObjectID);

    mat4 result = mat4(1.0f);
    
    result = result * mat4_cast(conjugate(transform->getAbsoluteRotation()));
    
    if(translationEnabled) {
        result = translate(result, -transform->getAbsolutePosition());
    }
    
    return result;
}

mat4 Camera::getViewProjectionMatrix(bool translationEnabled) {
    mat4 perspectiveMatrix = perspective(fov, aspect, near, far);
    
    mat4 viewMatrix = getViewMatrix(translationEnabled);
    
    mat4 viewProjection = perspectiveMatrix * viewMatrix;
    
    return viewProjection;
}

