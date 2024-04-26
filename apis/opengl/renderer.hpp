#pragma once

#define GL_SILENCE_DEPRECATION

#include <OpenGL/gl3.h>

#include <vector>

#include "render-object.hpp"
#include "../../core/components/mesh.hpp"
#include "../../core/components/terrain.hpp"
#include "../../core/components/cubic-patch.hpp"

#include "../base/irenderer.hpp"

#include "shader.hpp"
#include "resource-manager.hpp"

using namespace std;

class Renderer : public IRenderer {
public:
    Renderer();
    
    ~Renderer() override;

// ===
// Overrode methods
// ===
public:
    void setScene(Scene* scene) override;
    
    void render() override;
    
private:
    Shader baseShader;

    Shader goochShader;

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

    void setLights(Shader* shader);

    void drawMesh(IRenderObject* object);

    void drawTerrain(IRenderObject* object);

    void drawCubicPatch(IRenderObject* object);
};

