#pragma once

#include "../core/scene.hpp"

class BackpackDemo : public Scene {
public:
    BackpackDemo() = default;

    void setupScene() override;

    void beforeRender() override;

private:
    GameObject* backpack;

    GameObject* createBackpack();

    GameObject* createHelmet();

    GameObject* createTerrain();

    GameObject* createCurvedSurface();
};
