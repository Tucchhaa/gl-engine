#include "terrain.hpp"

Terrain::Terrain(Texture* terrainTexture, int resolution):
        terrainTexture(terrainTexture), resolution(resolution)
{
    vertices = calculateVertices();
}

vector<float> Terrain::calculateVertices() {
    auto width = (float)terrainTexture->width;
    auto height = (float)terrainTexture->height;
    auto _resolution = (float)resolution;

    vector<float> vertices;

    for(int i = 0; i < resolution; i++)
    {
        for(int j = 0; j < resolution; j++)
        {
            vertices.push_back(-width / 2.0f + width * i / _resolution); // v.x
            vertices.push_back(0.0f); // v.y
            vertices.push_back(-height / 2.0f + height * j / _resolution); // v.z
            vertices.push_back(i / _resolution); // u
            vertices.push_back(j / _resolution); // v

            vertices.push_back(-width / 2.0f + width * (i + 1) / _resolution); // v.x
            vertices.push_back(0.0f); // v.y
            vertices.push_back(-height / 2.0f + height * j / _resolution); // v.z
            vertices.push_back((i + 1) / _resolution); // u
            vertices.push_back(j / _resolution); // v

            vertices.push_back(-width / 2.0f + width * i / _resolution); // v.x
            vertices.push_back(0.0f); // v.y
            vertices.push_back(-height / 2.0f + height * (j + 1) / _resolution); // v.z
            vertices.push_back(i / _resolution); // u
            vertices.push_back((j + 1) / _resolution); // v

            vertices.push_back(-width / 2.0f + width * (i + 1) / _resolution); // v.x
            vertices.push_back(0.0f); // v.y
            vertices.push_back(-height / 2.0f + height * (j + 1) / _resolution); // v.z
            vertices.push_back((i + 1) / _resolution); // u
            vertices.push_back((j + 1) / _resolution); // v
        }
    }

    return vertices;
}


Texture* Terrain::getTexture() {
    return terrainTexture;
}

// === IMesh implementation ===

void* Terrain::data() {
    return vertices.data();
}

int Terrain::dataSize() {
    return static_cast<int>(vertices.size()) * sizeof(float);
}

vector<int> Terrain::shaderAttributes() {
    return {3, 2};
}

bool Terrain::usesTessellation() {
    return true;
}

int Terrain::getVertexCount() {
    return VERTICES_PER_PATCH * resolution * resolution;
}

int Terrain::getVerticesPerPatch() {
    return VERTICES_PER_PATCH;
}
