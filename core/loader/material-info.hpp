#pragma once

#include <map>

#include <assimp/material.h>

#include "../structures/vectors.hpp"

using namespace std;

class MaterialInfo {
public:
    const aiMaterial* material;

    static void print(const aiMaterial* material);

    MaterialInfo(const aiMaterial* material);

public:
    string name = "No name";

    Vec3 diffuseColor;
    Vec3 specularColor = Vec3(0, 0, 0);
    Vec3 ambientColor;
    Vec3 emissiveColor;
    Vec3 reflectiveColor;

    float roughness;
    float metalness;

    /**
     * ID: Type, value: path to texture
     */
    map<aiTextureType, const char*> textures;

private:
    void collectInfo();

    void collectTextures();

    template<typename T>
    T getProperty(const char* key, unsigned int type, unsigned int idx);

    template<>
    string getProperty(const char* key, unsigned int type, unsigned int idx);

    template<>
    Vec3 getProperty(const char* key, unsigned int type, unsigned int idx);

    template<>
    float getProperty(const char* key, unsigned int type, unsigned int idx);

public:
    void print();

    bool operator==(const MaterialInfo& other) const;

    bool operator<(const MaterialInfo& other) const;
private:
    void printTextures();

    template<typename T>
    void printProperty(const string& propertyName, const char* key, unsigned int type, unsigned int idx) const;
};
