#include "deferred-renderer.hpp"
#include "gl-check-error.hpp"

#include "../base/iengine.hpp"

#include "render-object.hpp"
#include "resource-manager.hpp"

DeferredRenderer::DeferredRenderer():
    meshShader("deferred/mesh.vert", "deferred/mesh.frag"),
    shadowMapShader("forward/depth-vertex.vert", "forward/depth-fragment.frag"),
    pointLightShader("deferred/lighting.vert", "deferred/pbr-point-light.frag"),
    directLightShader("deferred/lighting.vert", "deferred/pbr-direct-light.frag"),
    skyboxShader("deferred/skybox.vert", "deferred/skybox.frag"),
    cubicPatchShader("deferred/cubic-patch.vert", "deferred/mesh.frag", "deferred/cubic-patch.tesc", "deferred/cubic-patch.tese"),
    screenShader("deferred/screen.vert", "deferred/screen.frag")
{
    resourceManager = dynamic_cast<ResourceManager*>(IEngine::ResourceManager);

    initGBuffer();
    initLightingBuffer();
    initShadowFrameBuffer();

    initScreenVAO();
    initSphereVAO();
}

DeferredRenderer::~DeferredRenderer() {
    meshShader.deleteShader();
    shadowMapShader.deleteShader();
    pointLightShader.deleteShader();
    directLightShader.deleteShader();
    skyboxShader.deleteShader();
    cubicPatchShader.deleteShader();
    screenShader.deleteShader();

    glDeleteFramebuffers(1, &gBuffer);
    glDeleteFramebuffers(1, &lightingBuffer);

    for(unsigned int &shadowMapBuffer : shadowMapBuffers)
        glDeleteFramebuffers(1, &shadowMapBuffer);

    glDeleteTextures(1, &gDepth);
    glDeleteTextures(1, &gNormal);
    glDeleteTextures(1, &gAlbedoMetal);
    glDeleteTextures(1, &gAORoughness);
    glDeleteTextures(1, &lightedColor);
    glDeleteTextures(1, &cascadeShadowMaps);
}

void DeferredRenderer::setFrameSize(int width, int height) {
    IRenderer::setFrameSize(width, height);

    glDeleteFramebuffers(1, &gBuffer);
    glDeleteFramebuffers(1, &lightingBuffer);

    glDeleteTextures(1, &gDepth);
    glDeleteTextures(1, &gNormal);
    glDeleteTextures(1, &gAlbedoMetal);
    glDeleteTextures(1, &gAORoughness);
    glDeleteTextures(1, &lightedColor);

    initGBuffer();
    initLightingBuffer();
}

void DeferredRenderer::initGBuffer() {
    glGenFramebuffers(1, &gBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

    // - Normal color buffer
    glGenTextures(1, &gNormal);
    glBindTexture(GL_TEXTURE_2D, gNormal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, frameWidth, frameHeight, 0, GL_RGBA, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gNormal, 0);

    // - Albedo + Specular/Metallic buffer
    glGenTextures(1, &gAlbedoMetal);
    glBindTexture(GL_TEXTURE_2D, gAlbedoMetal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, frameWidth, frameHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gAlbedoMetal, 0);

    // - AO + Rougness buffer
    glGenTextures(1, &gAORoughness);
    glBindTexture(GL_TEXTURE_2D, gAORoughness);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, frameWidth, frameHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAORoughness, 0);

    // - Depth buffer
    glGenTextures(1, &gDepth);
    glBindTexture(GL_TEXTURE_2D, gDepth);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, frameWidth, frameHeight, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, gDepth, 0);

    constexpr unsigned int attachments[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
    glDrawBuffers(3, attachments);

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DeferredRenderer::initLightingBuffer() {
    glGenFramebuffers(1, &lightingBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, lightingBuffer);

    glGenTextures(1, &lightedColor);
    glBindTexture(GL_TEXTURE_2D, lightedColor);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, frameWidth, frameHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, lightedColor, 0);

    // - Share same depth texture with gBuffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, gDepth, 0);

    unsigned int attachments[] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, attachments);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        throw std::runtime_error("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DeferredRenderer::initShadowFrameBuffer() {
    // === Generate list of textures of cascade shadow maps
    glGenTextures(1, &cascadeShadowMaps);
    glBindTexture(GL_TEXTURE_2D_ARRAY, cascadeShadowMaps);
    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT,
        static_cast<int>(cascadeLevels.size()) - 1, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    constexpr float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BORDER_COLOR, borderColor);

    shadowMapBuffers.resize(cascadeLevels.size() - 1);

    for(int i=0; i < cascadeLevels.size() - 1; i++) {
        glGenFramebuffers(1, &shadowMapBuffers[i]);
        glBindFramebuffer(GL_FRAMEBUFFER, shadowMapBuffers[i]);

        glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, cascadeShadowMaps, 0, i);

        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
    }

    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
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

void DeferredRenderer::afterSceneSetup() {
    IRenderer::afterSceneSetup();

    const map<const Material*, vector<Mesh*>> meshesByMaterial = currentScene->getMeshesByMaterial();

    for(const auto& [material, meshes]: meshesByMaterial) {
        vector<IRenderObject*> list;

        for(Mesh* mesh: meshes) {
            auto* object = new RenderObject(mesh);
            list.push_back(object);
        }

        this->meshesByMaterial[material] = list;
    }

    // ===

    const vector<Mesh*> meshes = currentScene->getMeshes();
    const vector<Terrain*> terrains = currentScene->getTerrains();
    const vector<CubicPatch*> cubicPatches = currentScene->getCubicPatches();

    for(Mesh* mesh: meshes)
        this->meshes.push_back(new RenderObject(mesh));

    for(Terrain* terrain: terrains)
        this->terrains.push_back(new RenderObject(terrain));

    for(CubicPatch* cubicPatch: cubicPatches)
        this->cubicPatches.push_back(new RenderObject(cubicPatch));
}

void DeferredRenderer::beforeRender() {
    IRenderer::beforeRender();

    const vector<DirectLight*> directLights = currentScene->getDirectLights();

    if(!directLights.empty()) {
        cascadePlanes = getCascadePlanes();
        cascadePerspectives = getCascadePerspectives(directLights[0]);
    }
}

void DeferredRenderer::render() {
    renderShadowMap();

    renderGBuffer();
    renderLighting();
    renderSkybox();

    renderScreen();

    glCheckError();
}

// ===
// Render functions
// ===

void DeferredRenderer::renderGBuffer() {
    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
    glViewport(0,0, frameWidth, frameHeight);

    glEnable(GL_DEPTH_TEST);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    const mat4 viewProjection = currentScene->getCamera()->getViewProjectionMatrix();

    renderMeshes(viewProjection);
    renderCubicPatches(viewProjection);
}

void DeferredRenderer::renderLighting() {
    glBindFramebuffer(GL_FRAMEBUFFER, lightingBuffer);
    glViewport(0, 0, frameWidth, frameHeight);

    glDepthMask(GL_FALSE);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_STENCIL_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    renderPointLighting();
    renderDirectLighting();

    // === Reset to default

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_BLEND);
    glDepthMask(GL_TRUE);
}

void DeferredRenderer::renderShadowMap() {
    if(currentScene->getDirectLights().empty())
        return;

    // glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // glViewport(0, 0, frameWidth, frameHeight);
    // glEnable(GL_DEPTH_TEST);
    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_FRONT);
    //
    // glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    //
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // glBindFramebuffer(GL_FRAMEBUFFER, shadowMapBuffer);
    // glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    //
    // glEnable(GL_DEPTH_TEST);
    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_FRONT);
    //
    // glClear(GL_DEPTH_BUFFER_BIT);

    shadowMapShader.use();

    for(int i=0; i < cascadeLevels.size(); i++) {
        glBindFramebuffer(GL_FRAMEBUFFER, shadowMapBuffers[i]);
        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);

        glClear(GL_DEPTH_BUFFER_BIT);

        shadowMapShader.setMat4("perspective", cascadePerspectives[i]);

        for(auto* object: meshes) {
            const Mesh* mesh = object->getMesh<Mesh>();

            shadowMapShader.setMat4("transform", mesh->transform->getTransformMatrix());

            object->render();
        }
    }
}

void DeferredRenderer::renderPointLighting() {
    const Camera* camera = currentScene->getCamera();

    pointLightShader.use();
    pointLightShader.setTexture("gDepth", gDepth);
    pointLightShader.setTexture("gNormal", gNormal);
    pointLightShader.setTexture("gAlbedoMetallic", gAlbedoMetal);
    pointLightShader.setTexture("gAORoughness", gAORoughness);

    pointLightShader.setMat4("perspective", camera->getViewProjectionMatrix());
    pointLightShader.setVec3("cameraPos", camera->transform->getPosition());

    for(const auto* pointLight: currentScene->getPointLights()) {
        pointLightShader.setPointLight("light", pointLight);
        pointLightShader.setMat4("transform", calculatePointLightVolumeTransform(pointLight));

        glBindVertexArray(sphereVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}

void DeferredRenderer::renderDirectLighting() {
    if(currentScene->getDirectLights().empty())
        return;

    const Camera* camera = currentScene->getCamera();
    const DirectLight* directLight = currentScene->getDirectLights()[0];

    const mat4 cameraPerspective = camera->getViewProjectionMatrix();
    const mat4 lightVolumeTransform = calculateDirectLightVolumeTransform();

    directLightShader.use();
    directLightShader.setTexture("gDepth", gDepth);
    directLightShader.setTexture("gNormal", gNormal);
    directLightShader.setTexture("gAlbedoMetallic", gAlbedoMetal);
    directLightShader.setTexture("gAORoughness", gAORoughness);

    directLightShader.setInt("cascadeCount", static_cast<int>(cascadePlanes.size()));
    directLightShader.setTexture("shadowMap", cascadeShadowMaps, GL_TEXTURE_2D_ARRAY);
    directLightShader.setMat4Array("lightPerspectives", cascadePerspectives);
    directLightShader.setFloatArray("cascadePlaneDistances", cascadePlanes);

    directLightShader.setMat4("perspective", cameraPerspective);
    directLightShader.setVec3("cameraPos", camera->transform->getPosition());

    // TODO: supposed there is only one direct light, which also casts shadows
    directLightShader.setDirectLight("light", directLight);
    directLightShader.setMat4("transform", lightVolumeTransform);

    glBindVertexArray(sphereVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void DeferredRenderer::renderSkybox() {
    const Camera* camera = currentScene->getCamera();

    const CubeMap* cubeMap = resourceManager->getCubeMap(camera->cubeMap);

    if(cubeMap != nullptr) {
        glBindFramebuffer(GL_FRAMEBUFFER, lightingBuffer);

        glDisable(GL_CULL_FACE);

        glDepthMask(GL_FALSE);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_EQUAL);

        skyboxShader.use();
        skyboxShader.setMat4("rotationMatrix", camera->getViewProjectionMatrix(false));
        skyboxShader.setTexture("skybox", cubeMap->textureId, GL_TEXTURE_CUBE_MAP);

        glBindVertexArray(cubeMap->VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glBindVertexArray(0);
        glDepthFunc(GL_LESS);
        glDepthMask(GL_TRUE);
    }
}

void DeferredRenderer::renderScreen() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, frameWidth, frameHeight);

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

    for(const auto& [material, meshes]: meshesByMaterial) {
        meshShader.setMaterial(material);

        for(auto* object: meshes) {
            const Mesh* mesh = object->getMesh<Mesh>();
            const Transform* transform = mesh->transform;

            meshShader.setMat4("transform", transform->getTransformMatrix());
            meshShader.setMat3("normalTransform", transform->getNormalMatrix());

            object->render();
        }
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

mat4 DeferredRenderer::calculatePointLightVolumeTransform(const PointLight* light) {
    auto transform = mat4(1.0f);

    transform = translate(transform, light->transform->getPosition());
    transform = scale(transform, vec3(light->getRadius()));

    return transform;
}

mat4 DeferredRenderer::calculateDirectLightVolumeTransform() const {
    const Camera* camera = currentScene->getCamera();
    const mat4 transform = translate(mat4(1.0f), camera->transform->getPosition());

    return transform;
}

vector<float> DeferredRenderer::getCascadePlanes() const {
    vector<float> result(cascadeLevels.size() - 1);

    const Camera* camera = currentScene->getCamera();
    const float near = camera->near;
    const float far = camera->far;

    for(int i=0; i < cascadeLevels.size() - 1; i++) {
        const float cascadeFar = near + (far - near) * cascadeLevels[i+1];

        result[i] = cascadeFar;
    }

    return result;
}

vector<mat4> DeferredRenderer::getCascadePerspectives(const DirectLight* light) const {
    Camera* camera = currentScene->getCamera();

    const float originalNear = camera->near;
    const float originalFar = camera->far;

    vector<mat4> result;

    for(int i=0; i < cascadePlanes.size(); i++) {
        const float cascadeNear = i == 0 ? originalNear : cascadePlanes[i-1];
        const float cascadeFar = cascadePlanes[i];

        mat4 perspective = calculateCascadePerspective(light, cascadeNear, cascadeFar);

        result.push_back(perspective);
    }

    camera->near = originalNear;
    camera->far = originalFar;

    return result;
}

mat4 DeferredRenderer::calculateCascadePerspective(const DirectLight* light, const float near, const float far) const {
    Camera* camera = currentScene->getCamera();

    camera->near = near;
    camera->far = far;

    const mat4 perspectiveInverse = inverse(camera->getViewProjectionMatrix());

    // === Calculate light view
    vector<vec4> frustumCorners;
    auto center = vec3(0, 0, 0);

    for(int x=-1; x <= 1; x+=2) {
        for(int y=-1; y <= 1; y+=2) {
            for(int z=-1; z <= 1; z+=2) {
                vec4 point = perspectiveInverse * vec4(x, y, z, 1);
                point = point / point.w;

                frustumCorners.push_back(point);
                center += vec3(point);
            }
        }
    }

    center /= frustumCorners.size();

    const auto lightView = lookAt(center - light->transform->getDirectionVector(), center, vec3(0, 1, 0));

    // === Calculate light projection
    float minX = std::numeric_limits<float>::max();
    float maxX = std::numeric_limits<float>::lowest();
    float minY = std::numeric_limits<float>::max();
    float maxY = std::numeric_limits<float>::lowest();
    float minZ = std::numeric_limits<float>::max();
    float maxZ = std::numeric_limits<float>::lowest();
    for (const auto& v : frustumCorners)
    {
        const auto trf = lightView * v;
        minX = std::min(minX, trf.x);
        maxX = std::max(maxX, trf.x);
        minY = std::min(minY, trf.y);
        maxY = std::max(maxY, trf.y);
        minZ = std::min(minZ, trf.z);
        maxZ = std::max(maxZ, trf.z);
    }

    constexpr float zMult = 10.0f;
    minZ = minZ < 0 ? minZ * zMult : minZ / zMult;
    maxZ = maxZ < 0 ? maxZ / zMult : maxZ * zMult;

    const auto lightProjection = ortho(minX, maxX, minY, maxY, minZ, maxZ);

    return lightProjection * lightView;
}
