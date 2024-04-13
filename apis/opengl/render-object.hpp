#pragma once

#define GL_SILENCE_DEPRECATION

#include <vector>

#include <OpenGL/gl3.h>

#include "../../core/game-object/components/mesh.hpp"
#include "../../core/game-object/components/cubic-patch.hpp"
#include "../../core/game-object/components/terrain.hpp"

using namespace std;

class RenderObject {
private:
    unsigned int VBO = 0;
    unsigned int EBO = 0;

    IMesh* mesh;
public:

    unsigned int VAO = 0;

    RenderObject(IMesh* mesh);

    template<typename T>
    T* getMesh();

    void render();

private:
    void setup();

    void setupShaderAttributes() const;

    static int calculateStride(const vector<int>& shaderAttributes);
};

