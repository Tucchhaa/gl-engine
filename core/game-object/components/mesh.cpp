#include "mesh.hpp"

#include <utility>

Mesh::Mesh() = default;

Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices, Material material) :
    vertices(std::move(vertices)), indices(std::move(indices)), material(std::move(material))
{}

