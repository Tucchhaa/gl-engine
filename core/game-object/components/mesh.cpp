#include "mesh.hpp"

Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices, Material material) :
    vertices(vertices), indices(indices), material(material)
{}

