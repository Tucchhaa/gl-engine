#pragma once

#define GL_SILENCE_DEPRECATION

#include <OpenGL/gl3.h>

#include <vector>

#include "shader.hpp"

#include "../base/irenderer.hpp"

#include "../../core/game-object/components/mesh.hpp"

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
    
public:
    Renderer();
    
    ~Renderer();
    
    virtual void setScene(Scene* scene);
    
    virtual void render();
    
    virtual void setScreenSize(unsigned int width, unsigned int height);
    
private:
    Shader baseShader;
    
    Shader screenShader;
    
    unsigned int frameBuffer;
    
    unsigned int textureColorBuffer;
    
    unsigned int screenVAO;
    
private:
    void initFrameBuffer();
    
    void initScreenVAO();
    
    void setupMesh(Mesh* mesh);
    
    void drawMesh(MeshData* meshData);
};

