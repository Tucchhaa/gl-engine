#include "mesh.hpp"

Mesh::Mesh() = default;

Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices, const Material* material) :
    vertices(std::move(vertices)), indices(std::move(indices)), material(material)
{}

// === IMesh implementation ===

void* Mesh::data() {
    return vertices.data();
}

int Mesh::dataSize() {
    return static_cast<int>(vertices.size()) * sizeof(Vertex);
}

vector<int> Mesh::shaderAttributes() {
    return {3, 3, 3, 2};
}

bool Mesh::usesIndices() {
    return true;
}

void* Mesh::indicesData() {
    return indices.data();
}

int Mesh::indicesDataSize() {
    return static_cast<int>(indices.size()) * sizeof(unsigned int);
}

