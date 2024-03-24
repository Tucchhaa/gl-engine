#pragma once

#include <string>
#include <vector>
#include <utility>

using namespace std;

struct Texture {
private:
    static inline int lastID = 1;

    static int generateId() {
        return lastID++;
    }

public:
    int ID;
    string path;

    Texture(string path);
};

struct Material {
public:
    vector<Texture> specularTextures;
    vector<Texture> diffuseTextures;
};

