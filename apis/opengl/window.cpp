#define GL_SILENCE_DEPRECATION

#include "window.hpp"

#include <iostream>
#include <OpenGL/gl3.h>

Window::Window() = default;

// ===
// Overrode methods
// ===

void Window::create(int screenWidth, int screenHeight) {
    if (!glfwInit())
        exit(-1);

    IWindow::create(screenWidth, screenHeight);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);

    glfwWindow = glfwCreateWindow(screenWidth, screenHeight, "Hello World", nullptr, nullptr);

    if (!glfwWindow)
    {
        glfwTerminate();
        exit(-1);
    }

    glfwMakeContextCurrent(glfwWindow);

    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    getFrameBufferSize(frameWidth, frameHeight);
    glViewport(0, 0, frameWidth, frameHeight);
}

bool Window::isOpen() {
    return !glfwWindowShouldClose(glfwWindow);
}

void Window::pollEvents() {
    glfwSwapBuffers(glfwWindow);
    glfwPollEvents();
}

void Window::terminate() {
    glfwTerminate();
}

// ===

GLFWwindow* Window::getGLFWWindow() {
    return glfwWindow;
}

// ===

void Window::getFrameBufferSize(int &width, int &height) {
    glfwGetFramebufferSize(glfwWindow, &width, &height);
}
