#pragma once

#define GL_SILENCE_DEPRECATION

#include <OpenGL/gl3.h>

#include <vector>

#include "../../core/game-object/components/mesh.hpp"
#include "../../core/game-object/components/terrain.hpp"
#include "../../core/game-object/components/cubic-patch.hpp"

#include "../base/irenderer.hpp"

#include "shader.hpp"
#include "resource-manager.hpp"

using namespace std;

struct MeshData {
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    
    Mesh* mesh;
};

struct TerrainData {
    unsigned int VAO;
    unsigned int VBO;

    Terrain* terrain;
};

struct CubicPatchData {
    unsigned int VAO;
    unsigned int VBO;

    CubicPatch* cubicPatch;
};

class Renderer : public IRenderer {
private:
    Scene* currentScene;
    
    vector<MeshData> meshes;

    vector<TerrainData> terrains;

    vector<CubicPatchData> cubicPatches;
    
    int width = 2000;
    int height = 1600;

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

    // === TEMP
    bool isPolygonLineMode = false;

private:
    Shader baseShader;
    
    Shader screenShader;
    
    Shader skyboxShader;

    Shader terrainShader;

    Shader cubicPatchShader;
    
    unsigned int frameBuffer;
    
    unsigned int textureColorBuffer;
    
    unsigned int screenVAO;

private:
    void initFrameBuffer();
    
    void initScreenVAO();

    void setLights(Shader* shader);

    void setupMesh(Mesh* mesh);
    
    void drawMesh(MeshData* meshData);

    void setupTerrain(Terrain* terrain);

    void drawTerrain(TerrainData* terrainData);

    void setupCubicPatch(CubicPatch* cubicPatch);

    void drawCubicPatch(CubicPatchData* cubicPatchData);

};

