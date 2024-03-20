#pragma once

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

class Input {
private:
    GLFWwindow* window;
    
private:
    float yPositivePressed;
    float yNegativePressed;
    float xPositivePressed;
    float xNegativePressed;
    
    float lastFrame;
    float getDeltaTime();
    
public:
    Input(GLFWwindow* window);
    
public:
    int isShiftPressed;
    
    float deltaTime = 0;
    
    // ===
    
    void process();
    
    vec3 axisVec3();
    
    float axisHorizontal();
    
    float axisVertical();
};
