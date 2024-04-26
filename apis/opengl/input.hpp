#pragma once

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../base/iwindow.hpp"
#include "../base/iinput.hpp"

using namespace glm;

class Input : public IInput {
private:
    GLFWwindow* glfwWindow = nullptr;

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
    explicit Input(IWindow* window);

public:
    void process() override;

    // ===

    vec3 axisVec3() override;

    float axisHorizontal() override;

    float axisVertical() override;

    // ===

    bool isShiftPressed() override;

    float getDeltaTime() override;
};
