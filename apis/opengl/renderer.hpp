#pragma once

#define GL_SILENCE_DEPRECATION

#include <OpenGL/gl3.h>

#include <vector>

#include "render-object.hpp"
#include "../../core/game-object/components/mesh.hpp"
#include "../../core/game-object/components/terrain.hpp"
#include "../../core/game-object/components/cubic-patch.hpp"

#include "../base/irenderer.hpp"

#include "shader.hpp"
#include "resource-manager.hpp"

using namespace std;

class Renderer : public IRenderer {
private:
    Scene* currentScene;

    vector<RenderObject> meshes;

    vector<RenderObject> terrains;

    vector<RenderObject> cubicPatches;
    
    int screenWidth = 2000;
    int screenHeight = 1600;

public:
    Renderer();
    
    ~Renderer() override;

// ===
// Overrode methods
// ===
public:
    void setScene(Scene* scene) override;
    
    void render() override;
    
    void setScreenSize(int width, int height) override;
    
private:
    Shader baseShader;

    Shader depthShader;
    
    Shader screenShader;
    
    Shader skyboxShader;

    Shader terrainShader;

    Shader cubicPatchShader;

    unsigned int screenFrameBuffer;
    
    unsigned int textureColorBuffer;
    
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

    void drawMesh(RenderObject* object);

    void drawTerrain(RenderObject* object);

    void drawCubicPatch(RenderObject* object);
};

