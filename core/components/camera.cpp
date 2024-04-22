#include "camera.hpp"

#include "../hierarchy.hpp"

Camera::Camera(float fov, float near, float far)
    : fov(fov), near(near), far(far) {}

void Camera::setScreenSizes(const int screenWidth, const int screenHeight) {
    aspect = static_cast<float>(screenWidth) / static_cast<float>(screenHeight);
    
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;
}

/**
 * Returns the view matrix calculated using camera's position and rotation
 */
mat4 Camera::getViewMatrix(const bool needTranslation) const {
    auto result = mat4(1.0f);
    
    result = result * mat4_cast(conjugate(transform->getAbsoluteRotation()));
    
    if(needTranslation) {
        result = translate(result, -transform->getAbsolutePosition());
    }
    
    return result;
}

mat4 Camera::getViewProjectionMatrix(const bool needTranslation) const {
    const mat4 perspectiveMatrix = perspective(fov, aspect, near, far);
    
    const mat4 viewMatrix = getViewMatrix(needTranslation);
    
    const mat4 viewProjection = perspectiveMatrix * viewMatrix;
    
    return viewProjection;
}

