#include "iengine.hpp"

const string IEngine::RESOURCES_PATH = "/Users/tucha/Repositories/gl-engine/resources";

IWindow* IEngine::Window = nullptr;
IInput* IEngine::Input = nullptr;
Loader* IEngine::Loader = nullptr;
IResourceManager* IEngine::ResourceManager = nullptr;

PhysicsEngine* IEngine::PhysicsEngine;

IRenderer* IEngine::Renderer = nullptr;
Scene* IEngine::CurrentScene = nullptr;

Editor* IEngine::Editor;

vector<IGameEventsListener*> IEngine::gameEventsListeners;


IEngine::IEngine() {
    Editor = new ::Editor();

    PhysicsEngine = new ::PhysicsEngine();
}

void IEngine::setScene(Scene* scene) {
    CurrentScene = scene;

    scene->setupScene();

    Renderer->setScene(scene);
    Editor->setScene(scene);
}

void IEngine::invokeBeforeRender() {
    for(auto* listener : gameEventsListeners)
        listener->beforeRender();
}
