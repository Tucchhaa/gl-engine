#pragma once

#include "../base/irenderer.hpp"

#include "shader.hpp"

class DeferredRenderer : public IRenderer {
public:
    DeferredRenderer();

    ~DeferredRenderer() override;


private:
    void initGBuffer();

    void initLightingBuffer();

    void initScreenVAO();

    void initSphereVAO();

// ===
// Overrode methods
// ===
public:
    void setScene(Scene* scene) override;

    void render() override;

private:
    /**
     * Computes gBuffer textures
     */
    Shader sceneShader;

    /**
     * Computes lighting for gBuffer textures
     */
    Shader lightingShader;

    Shader skyboxShader;

    /**
     * Combines lighting buffers and outputs them the screen
     */
    Shader screenShader;

    unsigned int gBuffer = 0;
    unsigned int gDepthStencil = 0, gNormal = 0, gAlbedoSpec = 0;

    unsigned int lightingBuffer = 0;
    unsigned int lightDiffuse = 0, lightSpecular = 0;
    unsigned int lightedColor = 0;

    unsigned int screenVAO = 0;
    unsigned int sphereVAO = 0;

// ===
// Render functions
// ===
private:
    void renderGBuffer();

    void renderLighting();

    void renderSkybox();

    void renderScreen();

    void renderMeshes();

// ===
// Private methods
// ===
private:
    void setLights(Shader* shader) const;

    static mat4 calculateLightVolumeMatrix(const PointLight* light) ;
};
