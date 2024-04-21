#include "deferred-renderer.hpp"
#include "gl-check-error.hpp"

#include "render-object.hpp"

DeferredRenderer::DeferredRenderer():
    sceneShader("deferred/scene.vert", "deferred/scene.frag"),
    lightingShader("deferred/lighting.vert", "deferred/lighting.frag"),
    screenShader("deferred/screen.vert", "deferred/screen.frag")
{
    initGBuffer();
    initLightingBuffer();

    initScreenVAO();
    initSphereVAO();
}

DeferredRenderer::~DeferredRenderer() {
    sceneShader.deleteShader();
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
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
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

    // TODO: try to use texture instead of renderbuffer
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

void DeferredRenderer::initLightingBuffer() {
    glGenFramebuffers(1, &lightingBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, lightingBuffer);

    glGenTextures(1, &lightedColor);
    glBindTexture(GL_TEXTURE_2D, lightedColor);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screenWidth, screenHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, lightedColor, 0);

    unsigned int attachments[] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, attachments);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        throw std::runtime_error("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DeferredRenderer::initScreenVAO() {
    // pos + texCoord
    constexpr float vertices[] = {
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
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, nullptr);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(float)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void DeferredRenderer::initSphereVAO() {
    glGenVertexArrays(1, &sphereVAO);
    glBindVertexArray(sphereVAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

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

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

    int stride = 3 * sizeof(float);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
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
    renderLighting();
    renderScreen();

    glCheckError();
}

// ===
// Render functions
// ===

void DeferredRenderer::renderGBuffer() {
    glViewport(0, 0, screenWidth, screenHeight);
    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Camera* camera = currentScene->getCamera();

    sceneShader.use();
    sceneShader.setMat4("perspective", camera->getViewProjectionMatrix());

    renderMeshes();
}

void DeferredRenderer::renderLighting() {
    glViewport(0, 0, screenWidth, screenHeight);
    glBindFramebuffer(GL_FRAMEBUFFER, lightingBuffer);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    glBlendFunc(GL_ONE, GL_ONE);

    const Camera* camera = currentScene->getCamera();

    lightingShader.use();
    lightingShader.setTexture("gPosition", gPosition);
    lightingShader.setTexture("gNormal", gNormal);
    lightingShader.setTexture("gAlbedoSpec", gAlbedoSpec);

    lightingShader.setMat4("perspective", camera->getViewProjectionMatrix());
    lightingShader.setVec3("cameraPos", camera->transform->getPosition());

    for(const auto* pointLight: currentScene->getPointLights()) {
        glBindVertexArray(sphereVAO);

        lightingShader.setPointLight("light", pointLight);
        lightingShader.setMat4("transform", calculateLightVolumeMatrix(pointLight));

        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    // === Reset to default

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_BLEND);
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
    screenShader.setTexture("lightedColor", lightedColor);

    glBindVertexArray(screenVAO);

    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void DeferredRenderer::renderMeshes() {
    for(auto* object: meshes) {
        const Mesh* mesh = object->getMesh<Mesh>();
        const Transform* transform = mesh->transform;

        sceneShader.setMat4("transform", transform->getTransformMatrix());
        sceneShader.setMat3("normalTransform", transform->getNormalMatrix());

        sceneShader.setMaterial(&mesh->material);

        object->render();
    }
}

// ===
// Private methods
// ===

void DeferredRenderer::setLights(const Shader* shader) const {
    for(int i=0; i < currentScene->getSpotLights().size(); i++) {
        shader->setSpotLight(i, currentScene->getSpotLights()[i]);
    }

    // for(int i=0; i < currentScene->getPointLights().size(); i++) {
    //     shader->setPointLight(i, currentScene->getPointLights()[i]);
    // }

    for(int i=0; i < currentScene->getDirectLights().size(); i++) {
        shader->setDirectLight(i, currentScene->getDirectLights()[i]);
    }
}

mat4 DeferredRenderer::calculateLightVolumeMatrix(const PointLight* light) {
    auto lightVolumeMatrix = mat4(1.0f);

    lightVolumeMatrix = translate(lightVolumeMatrix, light->transform->getPosition());
    lightVolumeMatrix = scale(lightVolumeMatrix, vec3(light->getRadius()));

    return lightVolumeMatrix;
}
