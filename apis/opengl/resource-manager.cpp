#include "resource-manager.hpp"
#include "gl-check-error.hpp"

ResourceManager::ResourceManager() = default;

// ===
// Getters
// ===
unsigned int ResourceManager::getTextureId(Texture* texture) {
    auto textureIt = getInstance().textures.find(texture->ID);

    if(textureIt != getInstance().textures.end())
        return textureIt->second;

    return 0;
}

unsigned int ResourceManager::getTextureId(const Material* material, glApiTextureTypes textureType) {
    switch(textureType) {
        case glApi_DIFFUSE_TEXTURE:
            return material->diffuseTexture.ID;
        case glApi_SPECULAR_TEXTURE:
            return material->specularTexture.ID;
        case glApi_NORMAL_TEXTURE:
            return material->normalTexture.ID;
        case glApi_ROUGHNESS_TEXTURE:
            return material->roughnessTexture.ID;
        case glApi_AO_TEXTURE:
            return material->aoTexture.ID;
        default:
            throw runtime_error("can not get texture");
    }
}

CubeMap* ResourceManager::getCubeMap(const Texture* texture) {
    ResourceManager* instance = &getInstance();
    auto iterator = instance->cubeMaps.find(texture->ID);

    if(iterator == instance->cubeMaps.end())
        return nullptr;

    return &iterator->second;
}


// ===
// Override methods
// ===

void ResourceManager::handleTexture(const Texture* texture) {
    unsigned int textureId;

    TextureOptions options = texture->options;
    int format = convertTextureFormat(texture->format);

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, convertTextureWrap(options.xWrap));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, convertTextureWrap(options.yWrap));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, convertTextureFilter(options.minFilter));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, convertTextureFilter(options.magFilter));

    glTexImage2D(GL_TEXTURE_2D, 0, format, texture->width, texture->height, 0, format, GL_UNSIGNED_BYTE, texture->data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    textures[texture->ID] = textureId;
}

void ResourceManager::handleCubeMap(const Texture* texture) {
    float vertices[] = {
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f,  1.0f
    };

    TextureOptions options = texture->options;

    CubeMap cubeMap;
    unsigned int VBO;
    glGenVertexArrays(1, &cubeMap.VAO);
    glBindVertexArray(cubeMap.VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

    int stride = 3 * sizeof(float);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)nullptr);
    glEnableVertexAttribArray(0);

    glGenTextures(1, &cubeMap.textureId);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap.textureId);

    const int CUBE_MAP_FACE_NUMBER = 6;

    int format = convertTextureFormat(texture->format);
    vector<unsigned char*>* _data = texture->getCubeMapData();

    for(unsigned int i = 0; i < CUBE_MAP_FACE_NUMBER; i++) {
        glTexImage2D(
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, format, texture->width, texture->height, 0, format, GL_UNSIGNED_BYTE, (*_data)[i]
        );
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, convertTextureFilter(options.magFilter));
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, convertTextureFilter(options.minFilter));
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, convertTextureWrap(options.xWrap));
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, convertTextureWrap(options.yWrap));
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, convertTextureWrap(options.zWrap));

    cubeMaps[texture->ID] = cubeMap;

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void ResourceManager::handleModel(const string &path) {
    // TODO: maybe call this method when load model? Initialize render object here
}

// ===
// Converters
// ===

int ResourceManager::convertTextureWrap(TextureWrap wrap) {
    switch (wrap) {
        case TEXTURE_WRAP_REPEAT:
            return GL_REPEAT;
        case TEXTURE_WRAP_CLAMP_TO_EDGE:
            return GL_CLAMP_TO_EDGE;
        default:
            throw std::runtime_error("unsupported TEXTURE_WRAP");
    }
}

int ResourceManager::convertTextureFilter(TextureFilter filter) {
    switch (filter) {
        case TEXTURE_FILTER_NEAREST:
            return GL_NEAREST;
        case TEXTURE_FILTER_LINEAR:
            return GL_LINEAR;
        case TEXTURE_FILTER_LINEAR_MIPMAP_LINEAR:
            return GL_LINEAR_MIPMAP_LINEAR;
        default:
            throw std::runtime_error("unsupported TEXTURE_FILTER");
    }
}

int ResourceManager::convertTextureFormat(TextureFormat format) {
    switch (format) {
        case TEXTURE_FORMAT_R:
            return GL_RED;
        case TEXTURE_FORMAT_RGB:
            return GL_RGB;
        case TEXTURE_FORMAT_RGBA:
            return GL_RGBA;
        default:
            throw std::runtime_error("unsupported TEXTURE_FORMAT");
    }
}
