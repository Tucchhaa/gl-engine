#pragma once

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../base/iinput.hpp"

using namespace glm;

class Input : public IInput {
private:
    GLFWwindow* glfwWindow = nullptr;

private:
    bool isWindowFocused = false;

    float lastFrameTime = 0;
    float deltaTime = 0;

    float yPositivePressed = 0;
    float yNegativePressed = 0;
    float xPositivePressed = 0;
    float xNegativePressed = 0;

    bool shiftPressed = false;
    bool spacePressed = false;
    bool leftMousePressed = false;

    double mouseX = 0;
    double mouseY = 0;

public:
    Input();

public:
    void reset() override;

    void calculateDeltaTime() override;

    void process() override;

    // ===

    vec2 axisVec2() override;

    float axisHorizontal() override;

    float axisVertical() override;

    vec2 mouseDelta() override;

    // ===

    bool isShiftPressed() override;

    bool isSpacePressed() override;

    // ===

    bool isLeftMousePressed() override;

    float getDeltaTime() override;

private:
    void setCursorPositionToCenter();
};
