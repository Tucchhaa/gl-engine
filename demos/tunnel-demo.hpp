#pragma once
#include "../core/scene.hpp"

class TunnelDemo : public Scene {
public:
    TunnelDemo(Loader* loader);

    void setupScene() override;

    void beforeRender() override;

private:
    GameObject* createCube();
};
