#pragma once

#include "scene.hpp"

class Editor: public IGameEventsListener {
private:
    Scene* currentScene = nullptr;

public:
    Editor() = default;

    void afterSceneSetup() override;
};
