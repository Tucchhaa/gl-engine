#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../../hierarchy.hpp"

#include "transform.hpp"
#include "component.hpp"

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

    string cubeMap;

    Camera(float fov, float near, float far);
    
public:
    mat4 getViewMatrix(bool translationEnabled = true);

    mat4 getViewProjectionMatrix(bool translationEnabled = true);
    
    void setScreenSizes(float screenWidth, float screenHeight);
};
