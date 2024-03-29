#pragma once

#define GL_SILENCE_DEPRECATION

#include <OpenGL/gl3.h>

#include <vector>

#include "../../core/game-object/components/mesh.hpp"

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

class Renderer : public IRenderer {
private:
    Scene* currentScene;
    
    vector<MeshData> meshes;
    
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
    
private:
    Shader baseShader;
    
    Shader screenShader;
    
    Shader skyboxShader;

    Shader terrainShader;
    
    unsigned int frameBuffer;
    
    unsigned int textureColorBuffer;
    
    unsigned int screenVAO;
    
    unsigned int skyboxVAO;
    
    unsigned int skyboxTexture;
    
private:
    void initFrameBuffer();
    
    void initScreenVAO();
    
    void setupMesh(Mesh* mesh);
    
    void drawMesh(MeshData* meshData);

// TEMP
private:

    void initTerrain();

    unsigned int terrainVAO, terrainEBO, heightMapTextureId;
};

