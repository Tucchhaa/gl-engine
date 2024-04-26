// WARNING: Obsolote file

#include "renderer.hpp"

#include "gl-check-error.hpp"

const string shaderDir = "forward/";

Renderer::Renderer() :
    baseShader(shaderDir+"vertex.vert", shaderDir+"fragment.frag"),
    goochShader(shaderDir+"vertex.vert", shaderDir+"gooch-fragment.frag"),
    depthShader(shaderDir+"depth-vertex.vert", shaderDir+"depth-fragment.frag"),
    screenShader(shaderDir+"screen-vertex.vert", shaderDir+"screen-fragment.frag"),
    skyboxShader(shaderDir+"skybox-vertex.vert", shaderDir+"skybox-fragment.frag"),
    terrainShader(shaderDir+"terrain-vertex.vert", shaderDir+"terrain-fragment.frag", shaderDir+"terrain-tess-control.tesc", shaderDir+"terrain-tess-eval.tese"),
    cubicPatchShader(shaderDir+"cubic-vertex.vert", shaderDir+"fragment.frag", shaderDir+"cubic-tess-control.tesc", shaderDir+"cubic-tess-eval.tese")
{
    initScreenFrameBuffer();
    initScreenVAO();

    initShadowFrameBuffer();
}

Renderer::~Renderer() {
    baseShader.deleteShader();
    goochShader.deleteShader();
    depthShader.deleteShader();
    screenShader.deleteShader();
    skyboxShader.deleteShader();
    terrainShader.deleteShader();
    cubicPatchShader.deleteShader();
}

void Renderer::initShadowFrameBuffer() {
    glGenFramebuffers(1, &shadowMapFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);

    glGenTextures(1, &shadowMap);
    glBindTexture(GL_TEXTURE_2D, shadowMap);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
                 SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::initScreenFrameBuffer() {
    glGenFramebuffers(1, &screenFrameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, screenFrameBuffer);
    
    glGenTextures(1, &colorBuffer);
    glBindTexture(GL_TEXTURE_2D, colorBuffer);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, screenWidth, screenHeight, 0, GL_RGBA, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    unsigned int RBO;
    glGenRenderbuffers(1, &RBO);
    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screenWidth, screenHeight);
    
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBuffer, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);
    
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        throw std::runtime_error("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
    }
    
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
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

void Renderer::setScene(Scene *scene) {
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

void Renderer::renderShadowMap() {
    // glViewport(0, 0, screenWidth, screenHeight);
    // glEnable(GL_DEPTH_TEST);
    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_FRONT);
    //
    // glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    //
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    glClear(GL_DEPTH_BUFFER_BIT);

    DirectLight* light = currentScene->getDirectLights()[0];
    mat4 lightProjection = ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 7.5f);
    mat4 viewMatrix = mat4_cast(conjugate(Hierarchy::getTransform(light)->getAbsoluteRotation()));
    mat4 perspective = lightProjection * viewMatrix;

    depthShader.use();
    depthShader.setMat4("perspective", perspective);

    for(auto &meshData : meshes) {
        Mesh* mesh = meshData->getMesh<Mesh>();
        Transform* transform = Hierarchy::getTransform(mesh);

        depthShader.setMat4("transform", transform->getTransformMatrix());

        meshData->render();
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::renderMeshes() {
    // DirectLight* light = currentScene->getDirectLights()[0];
    // mat4 lightProjection = ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 7.5f);
    // mat4 viewMatrix = mat4_cast(conjugate(Hierarchy::getTransform(light)->getAbsoluteRotation()));
    // mat4 lightPerspective = lightProjection * viewMatrix;

    Camera* camera = currentScene->getCamera();

    // cubicPatchShader.use();
    // cubicPatchShader.setMat4("perspective", camera->getViewProjectionMatrix());
    // cubicPatchShader.setVec3("cameraPos", Hierarchy::getTransform(camera)->getAbsolutePosition());
    // // cubicPatchShader.setMat4("lightPerspective", lightPerspective);
    // // cubicPatchShader.setTexture("shadowMap", shadowMap);
    //
    // setLights(&cubicPatchShader);
    //
    // for(auto* cubicPatch : cubicPatches) {
    //     drawCubicPatch(cubicPatch);
    // }

    // draw terrain
    // terrainShader.use();
    // terrainShader.setMat4("perspective", camera->getViewProjectionMatrix());
    // terrainShader.setMat4("rotationMatrix", camera->getViewMatrix());
    //
    // for(auto* terrain: terrains) {
    //     drawTerrain(terrain);
    // }

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // ===

    goochShader.use();
    goochShader.setMat4("perspective", currentScene->getCamera()->getViewProjectionMatrix());
    goochShader.setVec3("cameraPos", Hierarchy::getTransform(camera)->getAbsolutePosition());
    // baseShader.setMat4("lightPerspective", lightPerspective);
    // baseShader.setTexture("shadowMap", shadowMap);

    setLights(&goochShader);

    for(auto* mesh : meshes) {
        drawMesh(mesh);
    }
}

void Renderer::renderCubeMap() {
    Camera* camera = currentScene->getCamera();

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
}

void Renderer::render() {
    currentScene->beforeRender();

    // renderShadowMap();
    glViewport(0, 0, screenWidth, screenHeight);
    glBindFramebuffer(GL_FRAMEBUFFER, screenFrameBuffer);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // ===

    renderCubeMap();
    renderMeshes();

    // ===

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // ===

    screenShader.use();
    screenShader.setTexture("screenTexture", colorBuffer);

    glBindVertexArray(screenVAO);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    glCheckError();

    currentScene->afterRender();
}

void Renderer::setLights(Shader* shader) {
    // TODO: size of lights exceed 5 then shader will not work

    // for(int i=0; i < currentScene->getSpotLights().size(); i++) {
    //     shader->setSpotLight(i, currentScene->getSpotLights()[i]);
    // }

    for(int i=0; i < currentScene->getPointLights().size(); i++) {
        // shader->setPointLight(i, currentScene->getPointLights()[i]);
        string index = to_string(i);
        shader->setPointLight("pointLights["+index+"]", currentScene->getPointLights()[i]);
    }

    // for(int i=0; i < currentScene->getDirectLights().size(); i++) {
    //     shader->setDirectLight(i, currentScene->getDirectLights()[i]);
    // }
}

void Renderer::drawMesh(IRenderObject* object) {
    const Mesh* mesh = object->getMesh<Mesh>();
    const Transform* transform = Hierarchy::getTransform(mesh);

    goochShader.setMat4("transform", transform->getTransformMatrix());
    goochShader.setMat3("normalTransform", transform->getNormalMatrix());

    goochShader.setMaterial(&mesh->material);

    object->render();
}

void Renderer::drawTerrain(IRenderObject* object) {
    auto* terrain = object->getMesh<Terrain>();
    const unsigned int heightMapTextureId = ResourceManager::getTextureId(terrain->getTexture());

    terrainShader.setTexture("heightMap", heightMapTextureId);

    object->render();
}

void Renderer::drawCubicPatch(IRenderObject* object) {
    auto* patch = object->getMesh<CubicPatch>();
    const Transform* transform = Hierarchy::getTransform(patch);

    cubicPatchShader.setMat4("transform", transform->getTransformMatrix());
    cubicPatchShader.setMat3("normalTransform", transform->getNormalMatrix());

    cubicPatchShader.setFloat("tessOuterLevel", patch->tessOuterLevel);
    cubicPatchShader.setFloat("tessInnerLevel", patch->tessInnerLevel);
    cubicPatchShader.setMaterial(&patch->material);

    object->render();
}
