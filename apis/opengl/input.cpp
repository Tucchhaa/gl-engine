#include "input.hpp"

Input::Input(IWindow* window) {
    lastFrameTime = (float)glfwGetTime();
    deltaTime = 0;

    glfwWindow = dynamic_cast<Window*>(window)->getGLFWWindow();
}

// ===

void Input::process() {
    if (glfwGetKey(glfwWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(glfwWindow, true);

    yPositivePressed = glfwGetKey(glfwWindow, GLFW_KEY_W) == GLFW_PRESS;
    yNegativePressed = glfwGetKey(glfwWindow, GLFW_KEY_S) == GLFW_PRESS;

    xPositivePressed = glfwGetKey(glfwWindow, GLFW_KEY_D) == GLFW_PRESS;
    xNegativePressed = glfwGetKey(glfwWindow, GLFW_KEY_A) == GLFW_PRESS;

    _isShiftPressed = glfwGetKey(glfwWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS;

    calculateDeltaTime();

    // === TEMP ===
    bool isEPressedThisFrame = glfwGetKey(glfwWindow, GLFW_KEY_E) == GLFW_PRESS;
    isEPressed = isEPressedThisFrame && !wasEPressedLastFrame;
    wasEPressedLastFrame = isEPressedThisFrame;

    bool isQPressedThisFrame = glfwGetKey(glfwWindow, GLFW_KEY_Q) == GLFW_PRESS;
    isQPressed = isQPressedThisFrame && !wasQPressedLastFrame;
    wasQPressedLastFrame = isQPressedThisFrame;

    bool isSpacePressedThisFrame = glfwGetKey(glfwWindow, GLFW_KEY_SPACE) == GLFW_PRESS;
    isSpacePressed = isSpacePressedThisFrame && !wasSpacePressedLastFrame;
    wasSpacePressedLastFrame = isSpacePressedThisFrame;
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
