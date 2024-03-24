#include "renderer.hpp"
#include <iostream>
#include <stb_image.h>

#include "gl-check-error.hpp"

Renderer::Renderer() :
    baseShader("vertex.vert", "fragment.frag"),
    screenShader("screen-vertex.vert", "screen-fragment.frag"),
    skyboxShader("skybox-vertex.vert", "skybox-fragment.frag")
{
    initFrameBuffer();
    initScreenVAO();
}

Renderer::~Renderer() {
    baseShader.deleteShader();
    screenShader.deleteShader();
    skyboxShader.deleteShader();
}

void Renderer::initFrameBuffer() {
    glGenFramebuffers(1, &frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    
    glGenTextures(1, &textureColorBuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorBuffer);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    unsigned int RBO;
    glGenRenderbuffers(1, &RBO);
    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorBuffer, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);
    
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        throw std::runtime_error("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
    }
        
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::initScreenVAO() {
    // pos + texCoord
    float vertices[] = {
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
        1.0f, -1.0f,   1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
        1.0f, -1.0f,   1.0f, 0.0f,
        1.0f,  1.0f,   1.0f, 1.0f
    };
    
    glGenVertexArrays(1, &screenVAO);
    
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    
    glBindVertexArray(screenVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
    
    unsigned int stride = 4 * sizeof(float);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(float)));
    
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    
    glBindVertexArray(0);
}

void Renderer::setScene(Scene *scene) {
    currentScene = scene;
    vector<Mesh*> meshes = scene->getMeshes();
    
    for(Mesh* mesh: meshes) {
        setupMesh(mesh);
    }
}

void Renderer::setScreenSize(unsigned int width, unsigned int height) {
    this->width = width;
    this->height = height;
}

void Renderer::render() {
    Camera* camera = currentScene->getCamera();

    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glCheckError();
    
    // ===
    CubeMap* cubeMap = ResourceManager::getCubeMap(camera->cubeMap);
    if(cubeMap != nullptr) {
        skyboxShader.use();

        skyboxShader.setMat4("rotationMatrix", camera->getViewProjectionMatrix(false));

        glDepthMask(GL_FALSE);

        glBindVertexArray(cubeMap->VAO);

        glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap->textureId);

        glDrawArrays(GL_TRIANGLES, 0, 36);

        glDepthMask(GL_TRUE);
    }
    glCheckError();

    // ===
    
    baseShader.use();

    baseShader.setMat4("perspective", currentScene->getCamera()->getViewProjectionMatrix());
    baseShader.setVec3("cameraPos", Hierarchy::getTransform(currentScene->getCamera())->position);

    baseShader.setPointLight(0, currentScene->getPointLights()[0]);


    for(int i=0; i < meshes.size(); i++) {
        drawMesh(&meshes[i]);
    }

//    // ===
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // ===
    
    screenShader.use();
    
    glBindVertexArray(screenVAO);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureColorBuffer);
    screenShader.setInt("screenTexture", 0);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glCheckError();
}

void Renderer::setupMesh(Mesh* mesh) {
    MeshData meshData;
    meshData.mesh = mesh;
    
    glGenVertexArrays(1, &meshData.VAO);
    
    glGenBuffers(1, &meshData.VBO);
    glGenBuffers(1, &meshData.EBO);
    
    glBindVertexArray(meshData.VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, meshData.VBO);
    glBufferData(GL_ARRAY_BUFFER, mesh->vertices.size() * sizeof(Vertex), &mesh->vertices[0], GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshData.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indices.size() * sizeof(unsigned int), &mesh->indices[0], GL_STATIC_DRAW);
    
    int stride = sizeof(Vertex);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex, normal));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex, texCoords));
    
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    
    glBindVertexArray(0);
    
    meshes.push_back(meshData);
}

void Renderer::drawMesh(MeshData* meshData) {
    Transform* transform = Hierarchy::getTransform(meshData->mesh);
    
    baseShader.setMat4("transform", transform->getModelMatrix());
    baseShader.setMat3("normalTransform", transform->getNormalMatrix());
    
    baseShader.setMaterial(&meshData->mesh->material);

    glBindVertexArray(meshData->VAO);
    
    glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(meshData->mesh->indices.size()), GL_UNSIGNED_INT, 0);
    
    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);
}

