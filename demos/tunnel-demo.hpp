#pragma once
#include "../core/scene.hpp"

class TunnelDemo : public Scene {
public:
    TunnelDemo() = default;

    void setup() override;

    void beforeRender() override;
};
