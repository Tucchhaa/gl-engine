#pragma once

#include <glm/glm.hpp>

#include <string>
#include <vector>
#include <algorithm>

#include "component.hpp"
#include "material.hpp"

using namespace std;
using namespace glm;


struct Vertex {
    vec3 position;
    vec3 normal;
    vec2 texCoords;
};

class Mesh : public ObjectComponent {
public:
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    Material material;
    
    Mesh();
    
    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, Material materials);
};

