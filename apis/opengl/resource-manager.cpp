#include "resource-manager.hpp"
#include "gl-check-error.hpp"

ResourceManager::ResourceManager() = default;

// ===
// Getters
// ===
unsigned int ResourceManager::getTextureId(const Material* material, glApiTextureTypes textureType) {
    const vector<Texture>* textures;

    switch(textureType) {
        case glApi_DIFFUSE_TEXTURE:
            textures = &material->diffuseTextures;
            break;
        case glApi_SPECULAR_TEXTURE:
            textures = &material->specularTextures;
            break;
    }

    return getInstance().textures[(*textures)[0].path];
}

CubeMap* ResourceManager::getCubeMap(const string &path) {
    ResourceManager* instance = &getInstance();
    auto iterator = instance->cubeMaps.find(path);

    if(iterator == instance->cubeMaps.end())
        return nullptr;

    return &iterator->second;
}


// ===
// Override methods
// ===

bool ResourceManager::isResourceLoaded(ResourceType type, const string& path) {
    switch (type) {
        case RESOURCE_TEXTURE:
            return textures.count(path);
        case RESOURCE_CUBE_MAP:
            return cubeMaps.count(path);
        case RESOURCE_MODEL:
            return models.count(path);
    }
}

void ResourceManager::handleTexture(const string& path, unsigned char *data, TextureFormat format, int height, int width) {
    unsigned int textureId;

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    textures[path] = textureId;
}

void ResourceManager::handleCubeMap(
    const string& path, const vector<unsigned char *> *data,
    TextureFormat format, int height,  int width
) {
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

    vector<string> textures_faces = {
        "right.jpg",
        "left.jpg",
        "top.jpg",
        "bottom.jpg",
        "front.jpg",
        "back.jpg"
    };

    for(unsigned int i = 0; i < textures_faces.size(); i++) {
        glTexImage2D(
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, (*data)[i]
        );
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    cubeMaps[path] = cubeMap;

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void ResourceManager::handleModel(const string &path) {

}
