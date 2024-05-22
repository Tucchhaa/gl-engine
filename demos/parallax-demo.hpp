#pragma once

#include "../core/scene.hpp"

class ParallaxDemo: public Scene {
public:
    void setup() override;

    void beforeRender() override;

    float heightScale = 0.1;

private:
    GameObject* light = nullptr;


    void pointLightController();

    void heightScaleController();
};
