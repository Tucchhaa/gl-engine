#include "iengine.hpp"

const string IEngine::RESOURCES_PATH = "/Users/tucha/Repositories/gl-engine/resources";

IWindow* IEngine::Window = nullptr;
IInput* IEngine::Input = nullptr;
Loader* IEngine::Loader = nullptr;
IResourceManager* IEngine::ResourceManager = nullptr;
IRenderer* IEngine::Renderer = nullptr;
