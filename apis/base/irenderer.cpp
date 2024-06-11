#include "irenderer.hpp"

#include "iengine.hpp"

void IRenderer::afterSceneSetup() {
    currentScene = IEngine::CurrentScene;
}

void IRenderer::setFrameSize(const int width, const int height) {
    frameWidth = width;
    frameHeight = height;
}
