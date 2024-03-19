#pragma once

#define GL_SILENCE_DEPRECATION

#include <OpenGL/gl3.h>

#include <vector>

#include "shader.hpp"
#include "../game-object/components/mesh.hpp"

using namespace std;

struct MeshData {
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    
    Mesh* mesh;
};

class Renderer {
public:
    vector<MeshData> meshes;
    
public:
    Renderer();
    
    void setupMesh(Mesh* mesh);
    
    void render(Shader* shader, Shader* screenShader);
    
private:
    unsigned int frameBuffer;
    
    unsigned int textureColorBuffer;
    
    unsigned int screenVAO;
    
private:
    void initFrameBuffer();
    
    void initScreenVAO();
    
    void drawMesh(Shader* shader, MeshData* meshData);
};

