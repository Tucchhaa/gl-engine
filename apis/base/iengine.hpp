#pragma once

#include "../../core/loader/loader.hpp"

#include "iwindow.hpp"
#include "iinput.hpp"
#include "iresource-manager.hpp"
#include "irenderer.hpp"

class IEngine {
public:
    static IWindow* Window;
    static IInput* Input;
    static Loader* Loader;
    static IResourceManager* ResourceManager;
    static IRenderer* Renderer;

    static const string RESOURCES_PATH;

public:
    IEngine() = default;

    virtual ~IEngine() = default;
};
