#include "iengine.hpp"

const string IEngine::RESOURCES_PATH = "/Users/tucha/Repositories/gl-engine/resources";

IWindow* IEngine::Window = nullptr;
IInput* IEngine::Input = nullptr;
Loader* IEngine::Loader = nullptr;
IResourceManager* IEngine::ResourceManager = nullptr;
IRenderer* IEngine::Renderer = nullptr;
Scene* IEngine::CurrentScene = nullptr;
Editor* IEngine::Editor;

IEngine::IEngine() {
    Editor = new ::Editor();
}

void IEngine::setScene(Scene* scene) {
    CurrentScene = scene;

    scene->setupScene();

    Renderer->setScene(scene);
    Editor->setScene(scene);
}
