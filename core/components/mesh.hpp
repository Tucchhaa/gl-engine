#pragma once

#include <string>
#include <vector>

#include "../structures/vectors.hpp"

#include "component.hpp"
#include "../imesh.hpp"
#include "../structures/material.hpp"

using namespace std;

struct Vertex {
    Vec3 position;
    Vec3 normal;
    Vec3 tangent = Vec3(0, 0, 0);
    Vec2 texCoords = Vec2(0, 0);
};

class Mesh : public ObjectComponent, public IMesh {
public:
    vector<Vertex> vertices;

    vector<unsigned int> indices;

    const Material* material;
    
    Mesh();
    
    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, const Material* material);

// === IMesh implementation ===
public:
    void* data() override;

    int dataSize() override;

    vector<int> shaderAttributes() override;

    bool usesIndices() override;

    void* indicesData() override;

    int indicesDataSize() override;
};

