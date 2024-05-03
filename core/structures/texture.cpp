#include "texture.hpp"

Texture::Texture() = default;

Texture::Texture(
        string path, int width, int height,
        TextureFormat format, void* data, TextureOptions options
):
        path(std::move(path)),
        width(width),
        height(height),
        format(format),
        data(data),
        options(options)
{
    ID = generateId();
}

unsigned char* Texture::getTextureData() const {
    return (unsigned char*)data;
}

vector<unsigned char*>* Texture::getCubeMapData() const {
    return (vector<unsigned char*>*)data;
}
