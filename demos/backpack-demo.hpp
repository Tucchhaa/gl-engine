#pragma once

#include "../core/scene.hpp"

class BackpackDemo : public Scene {
public:
    BackpackDemo() = default;

    void setup() override;

    void beforeRender() override;

    GameObject* backpack;

private:
    GameObject* createBackpack();

    GameObject* createHelmet();

    GameObject* createTerrain();

    GameObject* createCurvedSurface();
};
