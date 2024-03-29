#include "texture-options.hpp"

TextureOptions::TextureOptions() = default;

TextureOptions::TextureOptions(
        TextureWrap xWrap, TextureWrap yWrap, TextureWrap zWrap,
        TextureFilter minFilter, TextureFilter magFilter
): xWrap(xWrap), yWrap(yWrap), zWrap(zWrap), minFilter(minFilter), magFilter(magFilter)
{

}
