#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../../hierarchy.hpp"

#include "transform.hpp"
#include "component.hpp"

#include "../../structures/texture.hpp"

using namespace glm;

class Camera : public ObjectComponent {
private:
    float aspect;
    
public:
    float fov;
    float near;
    float far;
    
    int screenWidth;
    int screenHeight;

    Texture* cubeMap;

    Camera(float fov, float near, float far);
    
public:
    mat4 getViewMatrix(bool needTranslation = true);

    mat4 getViewProjectionMatrix(bool needTranslation = true);
    
    void setScreenSizes(float screenWidth, float screenHeight);
};
