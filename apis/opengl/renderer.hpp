#pragma once

#define GL_SILENCE_DEPRECATION


#include <vector>

#include "../base/irenderer.hpp"

#include "shader.hpp"

using namespace std;

class Renderer : public IRenderer {
public:
    Renderer();
    
    ~Renderer() override;

// ===
// Overrode methods
// ===
public:
    void afterSceneSetup() override;
    
    void render() override;
    
private:
    ResourceManager* resourceManager;

    Shader baseShader;

    Shader parallaxShader;

    Shader depthShader;
    
    Shader screenShader;
    
    Shader skyboxShader;

    Shader terrainShader;

    Shader cubicPatchShader;

    unsigned int screenFrameBuffer;
    
    unsigned int colorBuffer;
    
    unsigned int screenVAO;

private:
    void renderMeshes();

    void renderCubeMap();

private:
    // == TEMP ===
    const int SHADOW_WIDTH = 2048;
    const int SHADOW_HEIGHT = 2048;

    unsigned int shadowMapFBO;

    unsigned int shadowMap;

    void initShadowFrameBuffer();

    void renderShadowMap();

    // ===

    void initScreenFrameBuffer();
    
    void initScreenVAO();

    void setLights(Shader* shader) const;

    void drawMesh(IRenderObject* object);

    void drawTerrain(IRenderObject* object);

    void drawCubicPatch(IRenderObject* object);
};

