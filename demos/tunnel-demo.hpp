#pragma once
#include "../core/scene.hpp"

class TunnelDemo : public Scene {
public:
    TunnelDemo() = default;

    void setupScene() override;

    void beforeRender() override;
};
