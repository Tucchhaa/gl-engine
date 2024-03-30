#include "terrain.hpp"

Terrain::Terrain(Texture* terrainTexture, int patchResolution):
    terrainTexture(terrainTexture), patchResolution(patchResolution)
{ }

vector<float> Terrain::calculatePatches() {
    auto width = (float)terrainTexture->width;
    auto height = (float)terrainTexture->height;
    auto _resolution = (float)patchResolution;

    vector<float> patches;
    
    for(int i = 0; i < patchResolution; i++)
    {
        for(int j = 0; j < patchResolution; j++)
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

int Terrain::getPatchesNum() {
    return 4 * patchResolution * patchResolution;
}

Texture* Terrain::getTexture() {
    return terrainTexture;
}
