#pragma once

#include "../core/scene.hpp"

class CollisionDemo: public Scene {
public:
    void setupScene() override;

    void beforeRender() override;
};
