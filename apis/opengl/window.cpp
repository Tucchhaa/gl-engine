#include "window.hpp"

Window::Window() = default;

// ===
// Overrode methods
// ===

void Window::create(int screenWidth, int screenHeight) {
    if (!glfwInit())
        exit(-1);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);

    window = glfwCreateWindow(screenWidth, screenHeight, "Hello World", nullptr, nullptr);

    if (!window)
    {
        glfwTerminate();
        exit(-1);
    }

    glfwMakeContextCurrent(window);

    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    getFrameBufferSize();
}

bool Window::isOpen() {
    return !glfwWindowShouldClose(window);
}


void Window::onRendered() {
    glfwSwapBuffers(window);
    glfwPollEvents();
}

void Window::terminate() {
    glfwTerminate();
}

// ===

GLFWwindow* Window::getGLFWWindow() {
    return window;
}

// ===

void Window::getFrameBufferSize() {
    glfwGetFramebufferSize(window, &frameWidth, &frameHeight);
    glViewport(0, 0, frameWidth, frameHeight);
}
