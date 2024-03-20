#include "input.hpp"

Input::Input(GLFWwindow* window) : window(window) {
    lastFrame = (float)glfwGetTime();
}

void Input::process() {
    deltaTime = getDeltaTime();
    
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    yPositivePressed = glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS;
    yNegativePressed = glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS;
    
    xPositivePressed = glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS;
    xNegativePressed = glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS;
    
    isShiftPressed = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS;
}

vec3 Input::axisVec3() {
    return vec3(
        axisHorizontal(), 0, axisVertical()
    );
}

float Input::axisHorizontal() {
    return xPositivePressed - xNegativePressed;
}

float Input::axisVertical() {
    return yPositivePressed - yNegativePressed;
}

// = Private functions =

float Input::getDeltaTime() {
    float currentFrame = (float)glfwGetTime();
    float deltaTime = (float)glfwGetTime() - lastFrame;
    
    lastFrame = currentFrame;
    
    return deltaTime;
}
