#include "input.hpp"

#include "window.hpp"
#include "engine.hpp"

Input::Input(): IInput() {
    glfwWindow = dynamic_cast<Window*>(GlEngine::Window)->getGLFWWindow();
}

// ===

void Input::reset() {
    lastFrameTime = static_cast<float>(glfwGetTime());
    deltaTime = 0;

    // ===

    yPositivePressed = yNegativePressed = xPositivePressed = xNegativePressed = 0;
    shiftPressed = false;

    // ===

    glfwGetCursorPos(glfwWindow, &mouseX, &mouseY);
}

void Input::process() {
    isWindowFocused = glfwGetWindowAttrib(glfwWindow, GLFW_FOCUSED) == GLFW_TRUE;

    if(isWindowFocused == false) {
        reset();
        return;
    }

    // ===

    if (glfwGetKey(glfwWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(glfwWindow, true);

    // ===

    yPositivePressed = glfwGetKey(glfwWindow, GLFW_KEY_W) == GLFW_PRESS;
    yNegativePressed = glfwGetKey(glfwWindow, GLFW_KEY_S) == GLFW_PRESS;

    xPositivePressed = glfwGetKey(glfwWindow, GLFW_KEY_D) == GLFW_PRESS;
    xNegativePressed = glfwGetKey(glfwWindow, GLFW_KEY_A) == GLFW_PRESS;

    shiftPressed = glfwGetKey(glfwWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS;
    spacePressed = glfwGetKey(glfwWindow, GLFW_KEY_SPACE) == GLFW_PRESS;

    leftMousePressed = glfwGetMouseButton(glfwWindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;

    // ===

    glfwGetCursorPos(glfwWindow, &mouseX, &mouseY);
    setCursorPositionToCenter();
}

// ===
// Getters
// ===
vec2 Input::axisVec2() {
    return {
        axisHorizontal(), axisVertical()
    };
}

float Input::axisHorizontal() {
    return xPositivePressed - xNegativePressed;
}

float Input::axisVertical() {
    return yPositivePressed - yNegativePressed;
}

vec2 Input::mouseDelta() {
    return {
        static_cast<float>(mouseX - GlEngine::Window->screenWidth/2.0),
        static_cast<float>(mouseY - GlEngine::Window->screenHeight/2.0)
    };
}

bool Input::isShiftPressed() {
    return shiftPressed;
}

bool Input::isSpacePressed() {
    return spacePressed;
}

bool Input::isLeftMousePressed() {
    return leftMousePressed;
}

float Input::getDeltaTime() {
    return deltaTime;
}

void Input::setCursorPositionToCenter() {
    const int width = GlEngine::Window->screenWidth;
    const int height = GlEngine::Window->screenHeight;

    glfwSetCursorPos(glfwWindow, width / 2.0, height / 2.0);
}

// ===
// Private functions
// ===

void Input::calculateDeltaTime() {
    auto currentFrame = (float)glfwGetTime();

    deltaTime = currentFrame - lastFrameTime;

    lastFrameTime = currentFrame;
}
