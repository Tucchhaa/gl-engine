#include "material.hpp"

Texture::Texture(string path): path(std::move(path)) {
    ID = generateId();
}
