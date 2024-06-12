#include "iengine.hpp"
#include <iostream>

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

    scene->createCamera();
    scene->setup();

    for(const auto& listener : gameEventsListeners)
        listener->afterSceneSetup();
}

void IEngine::start() {
    Input->reset();

    while(Window->isOpen()) {
        Input->process();
        Input->calculateDeltaTime();

        fpsDisplay();
        cameraController();

        for(auto* listener : gameEventsListeners)
            listener->beforeRender();

        Renderer->render();

        for(auto* listener : gameEventsListeners)
            listener->afterRender();

        Window->pollEvents();
    }

    CurrentScene->finish();
    Window->terminate();
}

void IEngine::cameraController() {
    IInput* input = IEngine::Input;
    Transform* cameraTransform = CurrentScene->getCamera()->transform;

    constexpr float rotationSpeed = 0.3f;
    constexpr float speed = 15.0f;

    const float deltaX = -input->mouseDelta().x;
    const float deltaY = -input->mouseDelta().y;

    const auto horizontalRotation = quat(vec3(0,  deltaX * rotationSpeed * input->getDeltaTime(), 0));
    const auto verticalRotation   = quat(vec3(deltaY * rotationSpeed * input->getDeltaTime(), 0, 0));

    const vec2 translation = input->axisVec2() * speed * input->getDeltaTime();

    cameraTransform->rotate(horizontalRotation, &Transform::World);
    cameraTransform->rotate(verticalRotation);

    cameraTransform->translate(vec3(translation.x, 0, translation.y));
}

void IEngine::fpsDisplay() {
    static int frameCnt = 0;
    static time_t lastTime = time(nullptr);

    frameCnt++;

    time_t now = time(nullptr);
    time_t diff = difftime(now, lastTime);

    if(diff >= 1) {
        std::cout << "FPS: " << frameCnt << std::endl;
        frameCnt = 0;
        lastTime = now;
    }
}

