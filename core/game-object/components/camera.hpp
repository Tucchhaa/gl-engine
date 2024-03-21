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
    
    Camera(float fov, float near, float far);
    
private:
    mat4 getViewMatrix();

public:
    mat4 getViewProjectionMatrix();
    
    void setScreenSizes(float screenWidth, float screenHeight);
};
