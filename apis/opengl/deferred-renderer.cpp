#include "deferred-renderer.hpp"
#include "gl-check-error.hpp"

#include "render-object.hpp"
#include "resource-manager.hpp"
#include "../../../../../../Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX14.0.sdk/System/Library/Frameworks/OpenGL.framework/Headers/gl.h"

DeferredRenderer::DeferredRenderer():
    meshShader("deferred/mesh.vert", "deferred/mesh.frag"),
    // lightingShader("deferred/lighting.vert", "deferred/lighting.frag"),
    lightingShader("deferred/lighting.vert", "deferred/pbr.frag"),
    skyboxShader("deferred/skybox.vert", "deferred/skybox.frag"),
    cubicPatchShader("deferred/cubic-patch.vert", "deferred/mesh.frag", "deferred/cubic-patch.tesc", "deferred/cubic-patch.tese"),
    screenShader("deferred/screen.vert", "deferred/screen.frag")
{
    initGBuffer();
    initLightingBuffer();

    initScreenVAO();
    initSphereVAO();
}

DeferredRenderer::~DeferredRenderer() {
    meshShader.deleteShader();
}

void DeferredRenderer::initGBuffer() {
    glGenFramebuffers(1, &gBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

    // - Normal color buffer
    glGenTextures(1, &gNormal);
    glBindTexture(GL_TEXTURE_2D, gNormal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, screenWidth, screenHeight, 0, GL_RGBA, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gNormal, 0);

    // - Albedo + Specular/Metallic buffer
    glGenTextures(1, &gAlbedoMetal);
    glBindTexture(GL_TEXTURE_2D, gAlbedoMetal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screenWidth, screenHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gAlbedoMetal, 0);

    // - AO + Rougness buffer
    glGenTextures(1, &gAORoughness);
    glBindTexture(GL_TEXTURE_2D, gAORoughness);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screenWidth, screenHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAORoughness, 0);

    // - Depth buffer
    glGenTextures(1, &gDepthStencil);
    glBindTexture(GL_TEXTURE_2D, gDepthStencil);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, screenWidth, screenHeight, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, gDepthStencil, 0);

    constexpr unsigned int attachments[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
    glDrawBuffers(3, attachments);

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

    constexpr float vertices[] = {
        1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,

        1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,

        -1.0f, -1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, -1.0f, // 1
        -1.0f, -1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,

        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, 1.0f,

        1.0f, -1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
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

    const vector<Mesh*> meshes = scene->getMeshes();
    const vector<Terrain*> terrains = scene->getTerrains();
    const vector<CubicPatch*> cubicPatches = scene->getCubicPatches();

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
    currentScene->beforeRender();

    glViewport(0, 0, screenWidth, screenHeight);

    renderGBuffer();
    renderLighting();

    renderScreen();
    renderSkybox();

    glCheckError();

    currentScene->afterRender();
}

// ===
// Render functions
// ===

void DeferredRenderer::renderGBuffer() {
    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_STENCIL_TEST);
    glStencilMask(0xFF);
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    mat4 viewProjection = currentScene->getCamera()->getViewProjectionMatrix();

    renderMeshes(viewProjection);
    renderCubicPatches(viewProjection);
}

void DeferredRenderer::renderLighting() {
    glBindFramebuffer(GL_FRAMEBUFFER, lightingBuffer);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_STENCIL_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    const Camera* camera = currentScene->getCamera();

    lightingShader.use();
    lightingShader.setTexture("gDepth", gDepthStencil);
    lightingShader.setTexture("gNormal", gNormal);
    // lightingShader.setTexture("gAlbedoSpec", gAlbedoSpec);
    lightingShader.setTexture("gAlbedoMetallic", gAlbedoMetal);
    lightingShader.setTexture("gAORoughness", gAORoughness);

    lightingShader.setMat4("perspective", camera->getViewProjectionMatrix());
    lightingShader.setVec3("cameraPos", camera->transform->getPosition());

    for(const auto* pointLight: currentScene->getPointLights()) {
        lightingShader.setPointLight("light", pointLight);
        lightingShader.setMat4("transform", calculateLightVolumeMatrix(pointLight));

        glBindVertexArray(sphereVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    // === Reset to default

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_BLEND);
}

void DeferredRenderer::renderSkybox() {
    const Camera* camera = currentScene->getCamera();

    const CubeMap* cubeMap = ResourceManager::getCubeMap(camera->cubeMap);

    if(cubeMap != nullptr) {
        glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        glBlitFramebuffer(0, 0, screenWidth, screenHeight, 0, 0, screenWidth, screenHeight, GL_STENCIL_BUFFER_BIT, GL_NEAREST);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glEnable(GL_STENCIL_TEST);
        glStencilFunc(GL_EQUAL, 0, 0xFF);
        glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

        glDepthMask(GL_FALSE);

        skyboxShader.use();
        skyboxShader.setMat4("rotationMatrix", camera->getViewProjectionMatrix(false));
        skyboxShader.setTexture("skybox", cubeMap->textureId, GL_TEXTURE_CUBE_MAP);

        glBindVertexArray(cubeMap->VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glBindVertexArray(0);
        glDepthMask(GL_TRUE);
    }
}

void DeferredRenderer::renderScreen() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_STENCIL_TEST);
    glDisable(GL_CULL_FACE);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // ===

    screenShader.use();
    screenShader.setTexture("lightedColor", lightedColor);

    glBindVertexArray(screenVAO);

    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void DeferredRenderer::renderMeshes(const mat4& viewProjection) {
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    meshShader.use();
    meshShader.setMat4("perspective", viewProjection);

    for(auto* object: meshes) {
        const Mesh* mesh = object->getMesh<Mesh>();
        const Transform* transform = mesh->transform;

        meshShader.setMat4("transform", transform->getTransformMatrix());
        meshShader.setMat3("normalTransform", transform->getNormalMatrix());
        meshShader.setMaterial(&mesh->material);

        object->render();
    }
}

void DeferredRenderer::renderCubicPatches(const mat4& viewProjection) {
    glDisable(GL_CULL_FACE);

    cubicPatchShader.use();
    cubicPatchShader.setMat4("perspective", viewProjection);

    for(auto* object: cubicPatches) {
        const CubicPatch* patch = object->getMesh<CubicPatch>();
        const Transform* transform = patch->transform;

        cubicPatchShader.setMat4("transform", transform->getTransformMatrix());
        cubicPatchShader.setMat3("normalTransform", transform->getNormalMatrix());

        cubicPatchShader.setFloat("tessOuterLevel", patch->tessOuterLevel);
        cubicPatchShader.setFloat("tessInnerLevel", patch->tessInnerLevel);
        cubicPatchShader.setMaterial(&patch->material);

        object->render();
    }
}

// ===
// Private methods
// ===

void DeferredRenderer::setLights(Shader* shader) const {
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

// TODO: refactor
mat4 DeferredRenderer::calculateLightVolumeMatrix(const PointLight* light) {
    auto lightVolumeMatrix = mat4(1.0f);

    lightVolumeMatrix = translate(lightVolumeMatrix, light->transform->getPosition());
    lightVolumeMatrix = scale(lightVolumeMatrix, vec3(light->getRadius()));

    return lightVolumeMatrix;
}
