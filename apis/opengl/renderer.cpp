#include "renderer.hpp"

#include "gl-check-error.hpp"

Renderer::Renderer() :
    baseShader("vertex.vert", "fragment.frag"),
    screenShader("screen-vertex.vert", "screen-fragment.frag"),
    skyboxShader("skybox-vertex.vert", "skybox-fragment.frag"),
    terrainShader("terrain-vertex.vert", "terrain-fragment.frag", "terrain-tess-control.tesc", "terrain-tess-eval.tese"),
    cubicPatchShader("cubic-vertex.vert", "fragment.frag", "cubic-tess-control.tesc", "cubic-tess-eval.tese")
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
    currentScene = scene;
    vector<Mesh*> meshes = scene->getMeshes();
    vector<Terrain*> terrains = scene->getTerrains();
    vector<CubicPatch*> cubicPatches = scene->getCubicPatches();
    
    for(Mesh* mesh: meshes) {
        setupMesh(mesh);
    }

    for(Terrain* terrain: terrains) {
        setupTerrain(terrain);
    }

    for(CubicPatch* cubicPatch: cubicPatches) {
        setupCubicPatch(cubicPatch);
    }
}

void Renderer::setScreenSize(int width, int height) {
    this->width = width;
    this->height = height;
}

void Renderer::render() {
    Camera* camera = currentScene->getCamera();

    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

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

//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    cubicPatchShader.use();

    cubicPatchShader.setMat4("perspective", camera->getViewProjectionMatrix());
    cubicPatchShader.setVec3("cameraPos", Hierarchy::getTransform(camera)->getAbsolutePosition());
    cubicPatchShader.setSpotLight(0, currentScene->getSpotLights()[0]);

    for(auto &cubicPatch : cubicPatches) {
        drawCubicPatch(&cubicPatch);
    }

    glCheckError();

    // draw terrain
    terrainShader.use();

    terrainShader.setMat4("perspective", camera->getViewProjectionMatrix());
    terrainShader.setMat4("rotationMatrix", camera->getViewMatrix());

    for(auto &terrain: terrains) {
        drawTerrain(&terrain);
    }

    glEnable(GL_CULL_FACE);

    // ===

    baseShader.use();

    baseShader.setMat4("perspective", currentScene->getCamera()->getViewProjectionMatrix());
    baseShader.setVec3("cameraPos", Hierarchy::getTransform(camera)->getAbsolutePosition());
    baseShader.setSpotLight(0, currentScene->getSpotLights()[0]);

    for(auto &mesh : meshes) {
        drawMesh(&mesh);
    }

    // ===
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
    glBufferData(GL_ARRAY_BUFFER, (long)(mesh->vertices.size() * sizeof(Vertex)), &mesh->vertices[0], GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshData.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (long)(mesh->indices.size() * sizeof(unsigned int)), &mesh->indices[0], GL_STATIC_DRAW);
    
    int stride = sizeof(Vertex);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)nullptr);
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
    
    baseShader.setMat4("transform", transform->getTransformMatrix());
    baseShader.setMat3("normalTransform", transform->getNormalMatrix());
    
    baseShader.setMaterial(&meshData->mesh->material);

    glBindVertexArray(meshData->VAO);
    
    glDrawElements(GL_TRIANGLES, (int)static_cast<unsigned int>(meshData->mesh->indices.size()), GL_UNSIGNED_INT, 0);
    
    glBindVertexArray(0);
}

void Renderer::setupTerrain(Terrain* terrain) {
    TerrainData terrainData;
    vector<float> patches = terrain->calculatePatches();

    terrainData.terrain = terrain;

    glGenVertexArrays(1, &terrainData.VAO);
    glGenBuffers(1, &terrainData.VBO);

    glBindVertexArray(terrainData.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, terrainData.VBO);

    glBufferData(GL_ARRAY_BUFFER, (long)(patches.size() * sizeof(float)), &patches[0], GL_STATIC_DRAW);

    int stride = 5 * sizeof(float);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)nullptr); // xyz
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float))); // uv

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    terrains.push_back(terrainData);
}

void Renderer::drawTerrain(TerrainData* terrainData) {
    Terrain* terrain = terrainData->terrain;
    unsigned int heightMapTextureId = ResourceManager::getTextureId(terrain->getTexture());

    glBindVertexArray(terrainData->VAO);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, heightMapTextureId);
    terrainShader.setInt("heightMap", 0);

    glPatchParameteri(GL_PATCH_VERTICES, terrain->VERTICES_PER_PATCH);
    glDrawArrays(GL_PATCHES, 0, terrain->getVerticesCount());

    glBindVertexArray(0);
}

void Renderer::setupCubicPatch(CubicPatch* cubicPatch) {
    CubicPatchData cubicPatchData;
    cubicPatchData.cubicPatch = cubicPatch;

    const vector<float>* vertices = &cubicPatch->controlPoints;

    glGenVertexArrays(1, &cubicPatchData.VAO);
    glGenBuffers(1, &cubicPatchData.VBO);

    glBindVertexArray(cubicPatchData.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubicPatchData.VBO);

    glBufferData(GL_ARRAY_BUFFER, (long)(vertices->size() * sizeof(float)), vertices->data(), GL_STATIC_DRAW);

    int stride = 3 * sizeof(float);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, nullptr);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    cubicPatches.push_back(cubicPatchData);
}

void Renderer::drawCubicPatch(CubicPatchData* cubicPatchData) {
    Transform* transform = Hierarchy::getTransform(cubicPatchData->cubicPatch);
    CubicPatch* patch = cubicPatchData->cubicPatch;

    cubicPatchShader.setMat4("transform", transform->getTransformMatrix());
    cubicPatchShader.setMat3("normalTransform", transform->getNormalMatrix());

    cubicPatchShader.setFloat("tessOuterLevel", patch->tessOuterLevel);
    cubicPatchShader.setFloat("tessInnerLevel", patch->tessInnerLevel);

    cubicPatchShader.setMaterial(&patch->material);

    glBindVertexArray(cubicPatchData->VAO);

    glPatchParameteri(GL_PATCH_VERTICES, patch->VERTICES_PER_PATCH);
    glDrawArrays(GL_PATCHES, 0, patch->getVerticesCount());

    glBindVertexArray(0);
}
