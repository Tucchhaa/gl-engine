#pragma once

#include "../base/irenderer.hpp"

#include "shader.hpp"

class DeferredRenderer : public IRenderer {
private:
    ResourceManager* resourceManager;

public:
    DeferredRenderer();

    ~DeferredRenderer() override;

    void setFrameSize(int width, int height) override;
private:
    void initGBuffer();

    void initLightingBuffer();

    void initShadowFrameBuffer();

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

    Shader shadowMapShader;

    /**
     * Computes lighting for gBuffer textures
     */
    Shader pointLightShader;

    Shader directLightShader;

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

    unsigned int shadowMapBuffer = 0;
    unsigned int shadowMap = 0;
    const int SHADOW_WIDTH = 2048;
    const int SHADOW_HEIGHT = 2048;

    unsigned int screenVAO = 0;
    unsigned int sphereVAO = 0;

// ===
// Render functions
// ===
private:
    void renderGBuffer();

    void renderLighting();

    void renderShadowMap();

    void renderPointLighting();

    void renderDirectLighting();

    void renderSkybox();

    void renderScreen();

    void renderMeshes(const mat4& viewProjection);

    void renderCubicPatches(const mat4& viewProjection);

// ===
// Private methods
// ===
private:
    static mat4 calculatePointLightVolumeTransform(const PointLight* light);

    mat4 calculateDirectLightVolumeTransform() const;

    mat4 calculateShadowMapperPerspective(const DirectLight* light) const;
};
