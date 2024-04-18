#pragma once

#include "../base/irenderer.hpp"

#include "render-object.hpp"
#include "shader.hpp"

class DeferredRenderer : public IRenderer {
public:
    DeferredRenderer();

    ~DeferredRenderer() override;


private:
    void initGBuffer();

    void initScreenVAO();

// ===
// Overrode methods
// ===
public:
    void setScene(Scene* scene) override;

    void render() override;

private:
    Shader baseShader;

    Shader screenShader;

    unsigned int gBuffer = 0;
    unsigned int gPosition = 0, gNormal = 0, gAlbedoSpec = 0;

    unsigned int screenVAO = 0;

// ===
// Render functions
// ===
private:
    void renderGBuffer();

    void renderScreen();

    void renderMeshes();

// ===
// Private methods
// ===
private:
    void setLights(const Shader* shader) const;
};
