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
    Input->resetDeltaTime();

    while(Window->isOpen()) {
        Input->process();

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

    if(input->isShiftPressed()) {
        float rotationSpeed = 1.0f;

        quat horizontalRotation = quat(vec3(0, -input->axisHorizontal() * rotationSpeed * input->getDeltaTime(), 0));
        quat verticalRotation   = quat(vec3(-input->axisVertical() * rotationSpeed * input->getDeltaTime(), 0, 0));

        cameraTransform->rotate(horizontalRotation, &Transform::World);
        cameraTransform->rotate(verticalRotation);
    }
    else {
        float speed = 15.0f;

        cameraTransform->translate(input->axisVec3() * speed * input->getDeltaTime());
    }
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

