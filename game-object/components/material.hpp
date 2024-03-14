#pragma once

#include <vector>

using namespace std;

class Material {
public:
    vector<unsigned int> specularTextures;
    vector<unsigned int> diffuseTextures;
    
    Material();
};

