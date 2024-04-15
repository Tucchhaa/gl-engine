#include "render-object.hpp"

RenderObject::RenderObject(IMesh* mesh) {
    this->mesh = mesh;

    setup();
}

template<typename T>
T* RenderObject::getMesh() {
    return dynamic_cast<T*>(mesh);
}

void RenderObject::render() {
    glBindVertexArray(VAO);

    if(mesh->usesTessellation()) {
        glPatchParameteri(GL_PATCH_VERTICES, mesh->getVerticesPerPatch());
        glDrawArrays(GL_PATCHES, 0, mesh->getVertexCount());
    }
    else if(mesh->usesIndices()) {
        glDrawElements(GL_TRIANGLES, mesh->indicesDataSize(), GL_UNSIGNED_INT, nullptr);
    }
    // TODO: if not tessellation or indices, then draw arrays

    glBindVertexArray(0);
}

void RenderObject::setup() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, mesh->dataSize(), mesh->data(), GL_STATIC_DRAW);

    // const Mesh* mesh = dynamic_cast<Mesh*>(this->mesh);
    if(mesh->usesIndices()) {
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indicesDataSize(), mesh->indicesData(), GL_STATIC_DRAW);
    }

    setupShaderAttributes();

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void RenderObject::setupShaderAttributes() const {
    const vector<int> shaderAttributes = mesh->shaderAttributes();

    const int stride = calculateStride(shaderAttributes) * sizeof(float);
    int offset = 0;

    for(int i=0; i < shaderAttributes.size(); i++) {
        const int attribSize = shaderAttributes[i];

        glVertexAttribPointer(i, attribSize, GL_FLOAT, GL_FALSE, stride, (void*)offset);
        glEnableVertexAttribArray(i);

        offset += attribSize * static_cast<int>(sizeof(float));
    }
}

int RenderObject::calculateStride(const vector<int>& shaderAttributes) {
    int stride = 0;

    for (const int attribSize: shaderAttributes) {
        stride += attribSize;
    }

    return stride;
}


template Mesh* RenderObject::getMesh<Mesh>();
template CubicPatch* RenderObject::getMesh<CubicPatch>();
template Terrain* RenderObject::getMesh<Terrain>();