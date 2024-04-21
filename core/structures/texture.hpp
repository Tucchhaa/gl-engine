#pragma once

#include <string>
#include <vector>

#include "texture-options.hpp"

using namespace std;

enum TextureFormat {
    TEXTURE_FORMAT_R,
    TEXTURE_FORMAT_RGB,
    TEXTURE_FORMAT_RGBA,
};

struct Texture {
private:
    static inline int lastID = 1;

    static int generateId() {
        return lastID++;
    }

public:
    int ID;

    int width;
    int height;

    TextureFormat format;
    TextureOptions options;

    string path;
    void* data = nullptr;

    explicit Texture(string path, int width, int height, TextureFormat format, void* data, TextureOptions options = TextureOptions());

    unsigned char* getTextureData() const;

    vector<unsigned char*>* getCubeMapData() const;
};