#include "input.hpp"

#include "window.hpp"
#include "engine.hpp"

Input::Input(): IInput() {
    glfwWindow = dynamic_cast<Window*>(GlEngine::Window)->getGLFWWindow();
}

// ===

void Input::resetDeltaTime() {
    lastFrameTime = static_cast<float>(glfwGetTime());
    deltaTime = 0;
}

void Input::process() {
    if (glfwGetKey(glfwWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(glfwWindow, true);

    yPositivePressed = glfwGetKey(glfwWindow, GLFW_KEY_W) == GLFW_PRESS;
    yNegativePressed = glfwGetKey(glfwWindow, GLFW_KEY_S) == GLFW_PRESS;

    xPositivePressed = glfwGetKey(glfwWindow, GLFW_KEY_D) == GLFW_PRESS;
    xNegativePressed = glfwGetKey(glfwWindow, GLFW_KEY_A) == GLFW_PRESS;

    _isShiftPressed = glfwGetKey(glfwWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS;

    calculateDeltaTime();
}


// ===
// Getters
// ===
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

bool Input::isShiftPressed() {
    return _isShiftPressed;
}

float Input::getDeltaTime() {
    return deltaTime;
}

// ===
// Private functions
// ===

void Input::calculateDeltaTime() {
    auto currentFrame = (float)glfwGetTime();

    deltaTime = currentFrame - lastFrameTime;

    lastFrameTime = currentFrame;
}
