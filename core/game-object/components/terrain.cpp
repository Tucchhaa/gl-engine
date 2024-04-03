#include "terrain.hpp"

Terrain::Terrain(Texture* terrainTexture, int resolution):
        terrainTexture(terrainTexture), resolution(resolution)
{ }

vector<float> Terrain::calculatePatches() {
    auto width = (float)terrainTexture->width;
    auto height = (float)terrainTexture->height;
    auto _resolution = (float)resolution;

    vector<float> patches;
    
    for(int i = 0; i < resolution; i++)
    {
        for(int j = 0; j < resolution; j++)
        {
            patches.push_back(-width / 2.0f + width * i / _resolution); // v.x
            patches.push_back(0.0f); // v.y
            patches.push_back(-height / 2.0f + height * j / _resolution); // v.z
            patches.push_back(i / _resolution); // u
            patches.push_back(j / _resolution); // v

            patches.push_back(-width / 2.0f + width * (i + 1) / _resolution); // v.x
            patches.push_back(0.0f); // v.y
            patches.push_back(-height / 2.0f + height * j / _resolution); // v.z
            patches.push_back((i + 1) / _resolution); // u
            patches.push_back(j / _resolution); // v

            patches.push_back(-width / 2.0f + width * i / _resolution); // v.x
            patches.push_back(0.0f); // v.y
            patches.push_back(-height / 2.0f + height * (j + 1) / _resolution); // v.z
            patches.push_back(i / _resolution); // u
            patches.push_back((j + 1) / _resolution); // v

            patches.push_back(-width / 2.0f + width * (i + 1) / _resolution); // v.x
            patches.push_back(0.0f); // v.y
            patches.push_back(-height / 2.0f + height * (j + 1) / _resolution); // v.z
            patches.push_back((i + 1) / _resolution); // u
            patches.push_back((j + 1) / _resolution); // v
        }
    }

    return patches;
}

int Terrain::getVerticesCount() const {
    return VERTICES_PER_PATCH * resolution * resolution;
}

Texture* Terrain::getTexture() {
    return terrainTexture;
}
