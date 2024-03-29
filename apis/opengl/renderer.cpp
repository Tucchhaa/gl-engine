#include "renderer.hpp"

#include "gl-check-error.hpp"

#ifndef aboba
#define aboba
//#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#endif

Renderer::Renderer() :
    baseShader("vertex.vert", "fragment.frag"),
    screenShader("screen-vertex.vert", "screen-fragment.frag"),
    skyboxShader("skybox-vertex.vert", "skybox-fragment.frag"),
    terrainShader("terrain-vertex.vert", "terrain-fragment.frag", "terrain-tess-control.tesc", "terrain-tess-eval.tese")
{
    initFrameBuffer();
    initScreenVAO();
    initTerrain();
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
    
    int stride = 4 * sizeof(float);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(float)));
    
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    
    glBindVertexArray(0);
}

void Renderer::initTerrain() {
    int width, height, nChannels;
    unsigned char *data = stbi_load("/Users/tucha/Repositories/gl-engine/gl-engine/resources/textures/iceland_heightmap.png",
                                    &width, &height, &nChannels, 0);

    glGenTextures(1, &heightMapTextureId);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, heightMapTextureId); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(data);

    // vertex generation
    std::vector<float> vertices;

    unsigned rez = 20;
    for(unsigned i = 0; i < rez; i++)
    {
        for(unsigned j = 0; j < rez; j++)
        {
            vertices.push_back(-width/2.0f + width*i/(float)rez); // v.x
            vertices.push_back(0.0f); // v.y
            vertices.push_back(-height/2.0f + height*j/(float)rez); // v.z
            vertices.push_back(i / (float)rez); // u
            vertices.push_back(j / (float)rez); // v

            vertices.push_back(-width/2.0f + width*(i+1)/(float)rez); // v.x
            vertices.push_back(0.0f); // v.y
            vertices.push_back(-height/2.0f + height*j/(float)rez); // v.z
            vertices.push_back((i+1) / (float)rez); // u
            vertices.push_back(j / (float)rez); // v

            vertices.push_back(-width/2.0f + width*i/(float)rez); // v.x
            vertices.push_back(0.0f); // v.y
            vertices.push_back(-height/2.0f + height*(j+1)/(float)rez); // v.z
            vertices.push_back(i / (float)rez); // u
            vertices.push_back((j+1) / (float)rez); // v

            vertices.push_back(-width/2.0f + width*(i+1)/(float)rez); // v.x
            vertices.push_back(0.0f); // v.y
            vertices.push_back(-height/2.0f + height*(j+1)/(float)rez); // v.z
            vertices.push_back((i+1) / (float)rez); // u
            vertices.push_back((j+1) / (float)rez); // v
        }
    }

    // register VAO
    unsigned int terrainVBO;
    glGenVertexArrays(1, &terrainVAO);
    glBindVertexArray(terrainVAO);

    glGenBuffers(1, &terrainVBO);
    glBindBuffer(GL_ARRAY_BUFFER, terrainVBO);
    glBufferData(GL_ARRAY_BUFFER,
                 vertices.size() * sizeof(float),       // size of vertices buffer
                 &vertices[0],                          // pointer to first element
                 GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texCoord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(sizeof(float) * 3));
    glEnableVertexAttribArray(1);
}

void Renderer::setScene(Scene *scene) {
    currentScene = scene;
    vector<Mesh*> meshes = scene->getMeshes();
    
    for(Mesh* mesh: meshes) {
        setupMesh(mesh);
    }
}

void Renderer::setScreenSize(int width, int height) {
    this->width = width;
    this->height = height;
}

void Renderer::render() {
    Camera* camera = currentScene->getCamera();

//    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    glEnable(GL_DEPTH_TEST);
//    glEnable(GL_CULL_FACE);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glCheckError();

    // ===
//    CubeMap* cubeMap = ResourceManager::getCubeMap(camera->cubeMap);
//    if(cubeMap != nullptr) {
//        skyboxShader.use();
//
//        skyboxShader.setMat4("rotationMatrix", camera->getViewProjectionMatrix(false));
//
//        glDepthMask(GL_FALSE);
//
//        glBindVertexArray(cubeMap->VAO);
//
//        glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap->textureId);
//
//        glDrawArrays(GL_TRIANGLES, 0, 36);
//
//        glDepthMask(GL_TRUE);
//    }
//    glCheckError();


    // draw terrain
    terrainShader.use();

    terrainShader.setMat4("perspective", currentScene->getCamera()->getViewProjectionMatrix());
    terrainShader.setMat4("rotationMatrix", currentScene->getCamera()->getViewMatrix());

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, heightMapTextureId);
    terrainShader.setInt("heightMap", 0);

    int rez = 20;
    glPatchParameteri(GL_PATCH_VERTICES, 4);
    glBindVertexArray(terrainVAO);
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawArrays(GL_PATCHES, 0, 4*rez*rez);

    // ===

//    baseShader.use();
//
//    baseShader.setMat4("perspective", currentScene->getCamera()->getViewProjectionMatrix());
//    baseShader.setVec3("cameraPos", Hierarchy::getTransform(currentScene->getCamera())->getAbsolutePosition());
//
////    baseShader.setPointLight(0, currentScene->getPointLights()[0]);
//    baseShader.setSpotLight(0, currentScene->getSpotLights()[0]);
//
//    for(auto &mesh : meshes) {
//        drawMesh(&mesh);
//    }

//    // ===
//    glBindFramebuffer(GL_FRAMEBUFFER, 0);
//
//    glDisable(GL_DEPTH_TEST);
//    glDisable(GL_CULL_FACE);
//
//    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//    // ===
//
//    screenShader.use();
//
//    glBindVertexArray(screenVAO);
//
//    glActiveTexture(GL_TEXTURE0);
//    glBindTexture(GL_TEXTURE_2D, textureColorBuffer);
//    screenShader.setInt("screenTexture", 0);
//
//    glDrawArrays(GL_TRIANGLES, 0, 6);
//    glCheckError();
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
    
    baseShader.setMat4("transform", transform->getModelMatrix());
    baseShader.setMat3("normalTransform", transform->getNormalMatrix());
    
    baseShader.setMaterial(&meshData->mesh->material);

    glBindVertexArray(meshData->VAO);
    
    glDrawElements(GL_TRIANGLES, (int)static_cast<unsigned int>(meshData->mesh->indices.size()), GL_UNSIGNED_INT, 0);
    
    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);
}

