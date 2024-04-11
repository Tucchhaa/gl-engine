#pragma once

enum TextureWrap {
    TEXTURE_WRAP_REPEAT,
    TEXTURE_WRAP_CLAMP_TO_EDGE,
};

enum TextureFilter {
    TEXTURE_FILTER_NEAREST,
    TEXTURE_FILTER_LINEAR,
    TEXTURE_FILTER_LINEAR_MIPMAP_LINEAR,
};

struct TextureOptions {
public:
    TextureWrap xWrap = TEXTURE_WRAP_REPEAT;
    TextureWrap yWrap = TEXTURE_WRAP_REPEAT;
    TextureWrap zWrap = TEXTURE_WRAP_REPEAT;

    TextureFilter minFilter = TEXTURE_FILTER_NEAREST;
    TextureFilter magFilter = TEXTURE_FILTER_LINEAR;

    TextureOptions();

    TextureOptions(
            TextureWrap xWrap, TextureWrap yWrap, TextureWrap zWrap,
            TextureFilter minFilter, TextureFilter magFilter
    );
};

const TextureOptions CUBE_MAP_TEXTURE_OPTIONS = TextureOptions(
        TEXTURE_WRAP_CLAMP_TO_EDGE,
        TEXTURE_WRAP_CLAMP_TO_EDGE,
        TEXTURE_WRAP_CLAMP_TO_EDGE,
        TEXTURE_FILTER_LINEAR,
        TEXTURE_FILTER_LINEAR
);

const TextureOptions TEXTURE_2D_OPTIONS = TextureOptions(
        TEXTURE_WRAP_REPEAT,
        TEXTURE_WRAP_REPEAT,
        TEXTURE_WRAP_REPEAT,
        TEXTURE_FILTER_NEAREST,
        TEXTURE_FILTER_LINEAR
);

const TextureOptions TEXTURE_2D_OPTIONS_REPEAT = TextureOptions(
        TEXTURE_WRAP_REPEAT,
        TEXTURE_WRAP_REPEAT,
        TEXTURE_WRAP_REPEAT,
        TEXTURE_FILTER_NEAREST,
        TEXTURE_FILTER_NEAREST
);

const TextureOptions TERRAIN_OPTIONS = TextureOptions(
        TEXTURE_WRAP_REPEAT,
        TEXTURE_WRAP_REPEAT,
        TEXTURE_WRAP_REPEAT,
        TEXTURE_FILTER_LINEAR_MIPMAP_LINEAR,
        TEXTURE_FILTER_LINEAR
);
