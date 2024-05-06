#pragma once

#include <assimp/material.h>

using namespace std;

class MaterialInfo {
public:
    static void print(const aiMaterial* material);

private:
    template<typename T>
    static void printProperty(
        const aiMaterial* material, const string& propertyName, const char* key,
        unsigned int type, unsigned int idx
    );
};
