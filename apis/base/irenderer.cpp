#include "irenderer.hpp"

void IRenderer::setScene(Scene* scene) {
    currentScene = scene;
}

void IRenderer::setScreenSize(const int width, const int height) {
    screenWidth = width;
    screenHeight = height;
}
