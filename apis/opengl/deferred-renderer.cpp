//
// Created by Eldar Iusupzhanov on 2024/4/17.
//

#include "deferred-renderer.hpp"

#include "gl-check-error.hpp"

DeferredRenderer::DeferredRenderer():
    baseShader("deferred/vertex.vert", "deferred/fragment.frag"),
    screenShader("deferred/screen-vertex.vert", "deferred/screen-fragment.frag")
{
    initGBuffer();

    initScreenVAO();
}

DeferredRenderer::~DeferredRenderer() {
    baseShader.deleteShader();
}

void DeferredRenderer::initGBuffer() {
    glGenFramebuffers(1, &gBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

    // - Position color buffer
    glGenTextures(1, &gPosition);
    glBindTexture(GL_TEXTURE_2D, gPosition);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, screenWidth, screenHeight, 0, GL_RGBA, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);

    // - Normal color buffer
    glGenTextures(1, &gNormal);
    glBindTexture(GL_TEXTURE_2D, gNormal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, screenWidth, screenHeight, 0, GL_RGBA, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);

    // - Color + Specular color buffer
    glGenTextures(1, &gAlbedoSpec);
    glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screenWidth, screenHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedoSpec, 0);

    unsigned int attachments[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
    glDrawBuffers(3, attachments);

    // - Depth buffer
    unsigned int RBO;
    glGenRenderbuffers(1, &RBO);
    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, screenWidth, screenHeight);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RBO);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        throw std::runtime_error("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DeferredRenderer::initScreenVAO() {
    // pos + texCoord
    float vertices[] = {
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
        1.0f, -1.0f,   1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
        1.0f, -1.0f,   1.0f, 0.0f,
        1.0f,  1.0f,   1.0f, 1.0f
    };
    unsigned int VBO;

    glGenVertexArrays(1, &screenVAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(screenVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

    int stride = 4 * sizeof(float);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(float)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

// ===
// Overrode methods
// ===

void DeferredRenderer::setScene(Scene *scene) {
    IRenderer::setScene(scene);

    vector<Mesh*> meshes = scene->getMeshes();
    vector<Terrain*> terrains = scene->getTerrains();
    vector<CubicPatch*> cubicPatches = scene->getCubicPatches();

    for(Mesh* mesh: meshes) {
        auto* object = new RenderObject(mesh);
        this->meshes.push_back(object);
    }

    for(Terrain* terrain: terrains) {
        auto* object = new RenderObject(terrain);
        this->terrains.push_back(object);
    }

    for(CubicPatch* cubicPatch: cubicPatches) {
        auto* object = new RenderObject(cubicPatch);
        this->cubicPatches.push_back(object);
    }
}

void DeferredRenderer::render() {
    renderGBuffer();

    renderScreen();
}

// ===
// Render functions
// ===

void DeferredRenderer::renderGBuffer() {
    glViewport(0, 0, screenWidth, screenHeight);
    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    // glClearColor(0, 0, 0, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Camera* camera = currentScene->getCamera();

    baseShader.use();
    baseShader.setMat4("perspective", camera->getViewProjectionMatrix());

    renderMeshes();
}

void DeferredRenderer::renderScreen() {
    glViewport(0, 0, screenWidth, screenHeight);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // ===

    screenShader.use();
    screenShader.setTexture("gPosition", gPosition);
    screenShader.setTexture("gNormal", gNormal);
    screenShader.setTexture("gAlbedoSpec", gAlbedoSpec);

    setLights(&screenShader);

    glBindVertexArray(screenVAO);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    glCheckError();
}

void DeferredRenderer::renderMeshes() {
    for(auto* object: meshes) {
        const Mesh* mesh = object->getMesh<Mesh>();
        const Transform* transform = Hierarchy::getTransform(mesh);

        baseShader.setMat4("transform", transform->getTransformMatrix());
        baseShader.setMat3("normalTransform", transform->getNormalMatrix());

        baseShader.setMaterial(&mesh->material);

        object->render();
    }
}

// ===
// Private methods
// ===

void DeferredRenderer::setLights(const Shader* shader) const {
    // TODO: size of lights exceed 5 then shader will not work

    for(int i=0; i < currentScene->getSpotLights().size(); i++) {
        shader->setSpotLight(i, currentScene->getSpotLights()[i]);
    }

    for(int i=0; i < currentScene->getPointLights().size(); i++) {
        shader->setPointLight(i, currentScene->getPointLights()[i]);
    }

    for(int i=0; i < currentScene->getDirectLights().size(); i++) {
        shader->setDirectLight(i, currentScene->getDirectLights()[i]);
    }
}