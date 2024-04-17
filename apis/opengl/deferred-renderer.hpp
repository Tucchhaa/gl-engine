#pragma once

#include "../base/irenderer.hpp"

class DeferredRenderer : public IRenderer {
public:
    DeferredRenderer();

    ~DeferredRenderer() override;

// ===
// Overrode methods
// ===
public:
    void setScene(Scene* scene) override;

    void render() override;
};
