#pragma once

#include "../../core/loader/loader.hpp"

#include "iwindow.hpp"
#include "iinput.hpp"
#include "iresource-manager.hpp"
#include "irenderer.hpp"
#include "../../core/editor.hpp"

class IEngine {
public:
    static IWindow* Window;
    static IInput* Input;
    static Loader* Loader;
    static IResourceManager* ResourceManager;
    static IRenderer* Renderer;
    static Editor* Editor;
    static Scene* CurrentScene;

    static const string RESOURCES_PATH;

public:
    IEngine();

    virtual ~IEngine() = default;

    static void setScene(Scene* scene);
};
