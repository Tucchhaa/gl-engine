#pragma once

#include "iwindow.hpp"
#include "iinput.hpp"
#include "iresource-manager.hpp"
#include "irenderer.hpp"

#include "../../core/loader/loader.hpp"
#include "../../core/editor.hpp"
#include "../../core/physics-engine.hpp"

class IEngine {
public:
    static IWindow* Window;
    static IInput* Input;
    static Loader* Loader;
    static IResourceManager* ResourceManager;

    static PhysicsEngine* PhysicsEngine;

    static IRenderer* Renderer;
    static Editor* Editor;

    static Scene* CurrentScene;

    static const string RESOURCES_PATH;

    static vector<IGameEventsListener*> gameEventsListeners;

public:
    IEngine();

    virtual ~IEngine() = default;

    void setScene(Scene* scene);

    void start();
public:
    // TODO: remove this method
    static void cameraController();

    // TODO: remove this method
    static void fpsDisplay();
};
