#pragma once

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../base/iwindow.hpp"
#include "../base/iinput.hpp"

#include "window.hpp"

using namespace glm;

class Input : public IInput {
private:
    GLFWwindow* glfwWindow;

private:
    float yPositivePressed = 0;
    float yNegativePressed = 0;
    float xPositivePressed = 0;
    float xNegativePressed = 0;

    float lastFrameTime;
    float deltaTime;

    bool _isShiftPressed = false;

    void calculateDeltaTime();

public:
    Input(IWindow* window);

public:

    void process() override;

    // ===

    vec3 axisVec3() override;

    float axisHorizontal() override;

    float axisVertical() override;

    // ===

    bool isShiftPressed() override;

    float getDeltaTime() override;

    // === TEMP ===
    bool wasQPressedLastFrame;
    bool wasEPressedLastFrame;

    bool isQPressed = false;
    bool isEPressed = false;

};
