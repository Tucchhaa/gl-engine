#include "renderer.hpp"

void Renderer::setupMesh(Mesh* mesh) {
    MeshData meshData;
    meshData.mesh = mesh;
    
    glGenVertexArrays(1, &meshData.VAO);
    
    glGenBuffers(1, &meshData.VBO);
    glGenBuffers(1, &meshData.EBO);
    
    glBindVertexArray(meshData.VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, meshData.VBO);
    glBufferData(GL_ARRAY_BUFFER, mesh->vertices.size() * sizeof(Vertex), &mesh->vertices[0], GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshData.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indices.size() * sizeof(unsigned int), &mesh->indices[0], GL_STATIC_DRAW);
    
    int stride = sizeof(Vertex);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex, normal));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex, texCoords));
    
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    
    glBindVertexArray(0);
    
    meshes.push_back(meshData);
}

void Renderer::drawMesh(Shader* shader, MeshData* meshData) {
    shader->setMaterial(&meshData->mesh->material);

    glBindVertexArray(meshData->VAO);
    
    glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(meshData->mesh->indices.size()), GL_UNSIGNED_INT, 0);
    
    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);
}
