#include "irenderer.hpp"

#include "iengine.hpp"

void IRenderer::afterSceneSetup() {
    currentScene = IEngine::CurrentScene;
}

void IRenderer::setScreenSize(const int width, const int height) {
    screenWidth = width;
    screenHeight = height;
}
