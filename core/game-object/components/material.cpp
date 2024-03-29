#include "material.hpp"

Texture::Texture(string path, int width, int height, TextureFormat format, void* data):
    path(std::move(path)),
    width(width),
    height(height),
    format(format),
    data(data)
{
    ID = generateId();
}

unsigned char* Texture::getTextureData() const {
    return (unsigned char*)data;
}

vector<unsigned char*>* Texture::getCubeMapData() const {
    return (vector<unsigned char*>*)data;
}
