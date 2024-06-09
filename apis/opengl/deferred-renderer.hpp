#pragma once

#include "../base/irenderer.hpp"

#include "shader.hpp"

class DeferredRenderer : public IRenderer {
private:
    ResourceManager* resourceManager;

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
    void afterSceneSetup() override;

    void render() override;

private:
    /**
     * Computes gBuffer textures
     */
    Shader meshShader;

    /**
     * Computes lighting for gBuffer textures
     */
    Shader lightingShader;

    Shader skyboxShader;

    Shader cubicPatchShader;

    /**
     * Combines lighting buffers and outputs them the screen
     */
    Shader screenShader;

    unsigned int gBuffer = 0;
    unsigned int gDepthStencil = 0;
    unsigned int gNormal = 0, gAlbedoMetal = 0, gAORoughness = 0;

    unsigned int lightingBuffer = 0;
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

    void renderMeshes(const mat4& viewProjection);

    void renderCubicPatches(const mat4& viewProjection);

// ===
// Private methods
// ===
private:
    void setLights(Shader* shader) const;

    static mat4 calculateLightVolumeMatrix(const PointLight* light) ;
};
