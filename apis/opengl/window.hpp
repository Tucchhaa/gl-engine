#pragma once

#include <GLFW/glfw3.h>

#include "../base/iwindow.hpp"

class Window : public IWindow {
private:
    GLFWwindow* window = nullptr;

    int frameWidth = 0;
    int frameHeight = 0;

// ===
// Overrode methods
// ===
public:
    Window();

    void create(int screenWidth, int screenHeight) override;

    bool isOpen() override;

    void onRendered() override;

    void terminate() override;

public:
    GLFWwindow* getGLFWWindow();

private:
    void getFrameBufferSize();
};