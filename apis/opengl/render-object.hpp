#pragma once

#include <vector>

#include "../base/irender-object.hpp"

using namespace std;

class RenderObject : public IRenderObject {
private:
    unsigned int VAO = 0;
    unsigned int VBO = 0;
    unsigned int EBO = 0;

public:
    explicit RenderObject(IMesh* mesh);

    void render() override;

private:
    void setup();

    void setupShaderAttributes() const;

    static int calculateStride(const vector<int>& shaderAttributes);
};

