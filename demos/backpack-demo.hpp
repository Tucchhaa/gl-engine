#pragma once

#include "../core/scene.hpp"

class BackpackDemo : public Scene {
public:
    BackpackDemo(Loader* loader);

    void setupScene() override;

    void beforeRender() override;

private:
    GameObject* backpack;

    GameObject* createBackpack();

    GameObject* createHelmet();

    GameObject* createCube();

    GameObject* createTerrain();

    GameObject* createCurvedSurface();
};
