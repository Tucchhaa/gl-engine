#pragma once

#include "scene.hpp"

class Editor {
private:
    Scene* currentScene = nullptr;

public:
    Editor() = default;

    void setScene(Scene* scene);
};
