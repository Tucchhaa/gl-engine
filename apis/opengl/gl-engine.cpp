#include "gl-engine.hpp"

#include "deferred-renderer.hpp"
#include "window.hpp"
#include "input.hpp"
#include "resource-manager.hpp"


GlEngine::GlEngine(): IEngine() {
    constexpr int SCREEN_WIDTH = 1280;
    constexpr int SCREEN_HEIGHT = 720;

    IEngine::Window = new ::Window();

    Window->create(SCREEN_WIDTH, SCREEN_HEIGHT);

    IEngine::Input = new ::Input();
    IEngine::ResourceManager = new ::ResourceManager();
    IEngine::Loader = new ::Loader();
    IEngine::Renderer = new ::DeferredRenderer();

    init();
}

GlEngine::~GlEngine() {
    delete IEngine::Renderer;
    delete IEngine::Loader;
    delete IEngine::ResourceManager;
    delete IEngine::Input;
    delete IEngine::Window;
}

void GlEngine::init() {
    Hierarchy::initialize();
}
