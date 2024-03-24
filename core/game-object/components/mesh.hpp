#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <utility>


#include "../../structures/vectors.hpp"

#include "component.hpp"
#include "material.hpp"

using namespace std;

struct Vertex {
    Vec3 position;
    Vec3 normal;
    Vec2 texCoords;
};

class Mesh : public ObjectComponent {
public:
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    Material material;
    
    Mesh();
    
    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, Material materials);
};

