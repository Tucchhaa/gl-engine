#include "engine.hpp"

#include "deferred-renderer.hpp"
#include "editor-view-renderer.hpp"
#include "window.hpp"
#include "input.hpp"
#include "resource-manager.hpp"

GlEngine::GlEngine(): IEngine() {
    constexpr int SCREEN_WIDTH = 1280;
    constexpr int SCREEN_HEIGHT = 720;

    Window = new ::Window();

    Window->create(SCREEN_WIDTH, SCREEN_HEIGHT);

    Input = new ::Input();
    ResourceManager = new ::ResourceManager();
    Loader = new ::Loader();
    Renderer = new ::DeferredRenderer();
    // Renderer = new EditorViewRenderer();

    init();
}

GlEngine::~GlEngine() {
    delete Renderer;
    delete Loader;
    delete ResourceManager;
    delete Input;
    delete Window;
}

void GlEngine::init() {
    Hierarchy::initialize();

    int frameWidth, frameHeight;
    Window->getFrameBufferSize(frameWidth, frameHeight);
    Renderer->setFrameSize(frameWidth, frameHeight);
}
