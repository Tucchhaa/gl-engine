#include "backpack-demo.hpp"
#include "../core/hierarchy.hpp"

#include <iostream>

BackpackDemo::BackpackDemo(Loader* loader): Scene(loader) { }

void BackpackDemo::setupScene() {
    // GameObject* terrainObject = createTerrain();
    // GameObject* cubicPatchObject = createCurvedSurface();
    backpack = createBackpack();

    GameObject* helmet = createHelmet();
    GameObject* cubeObject = createCube();

    cubeObject->transform->scaleBy(vec3(100, 1, 100));
    cubeObject->transform->translate(vec3(0, -10, 0));

    // = light source =
    auto* lightSource = Hierarchy::createGameObject();

    Transform* lightTransform = lightSource->transform;
    lightTransform->translate(vec3(-8, 15, -10));
    lightTransform->rotate(vec3(0, radians(180.0), 0));
    lightTransform->rotate(vec3(radians(-30.0), 0, 0));

    auto* directLight0 = new DirectLight();
    PointLight* pointLight0 = PointLight::D3250();

    Hierarchy::addComponent(lightSource, directLight0);
    Hierarchy::addComponent(lightSource, pointLight0);

    // = Flashlight =
    // auto* flashlight = Hierarchy::createGameObject();
    //
    // auto* spotLight0 = SpotLight::D3250(radians(10.0));
    // Hierarchy::addComponent(flashlight, spotLight0);
    //
    // Hierarchy::setParent(cameraObject, flashlight);

    Scene::setupScene();
}

void BackpackDemo::beforeRender() {
    backpack->transform->rotate(quat(vec3(0, radians(0.15f), 0)));

    Hierarchy::updateTransformTree();

    Scene::beforeRender();
}

GameObject* BackpackDemo::createBackpack() {
    GameObject* object = loader->loadModel("models/backpack/backpack.fbx");

    auto* material = object->getData<Material>();

    material->diffuseTexture = *loader->loadTexture("models/backpack/albedo.jpg");
    material->normalTexture = *loader->loadTexture("models/backpack/normal.png");
    material->aoTexture = *loader->loadTexture("models/backpack/AO.jpg");
    material->roughnessTexture = *loader->loadTexture("models/backpack/roughness.jpg");
    material->specularTexture = *loader->loadTexture("models/backpack/metallic.jpg");

    Transform* objectTransform = object->transform;

    objectTransform->scaleBy(vec3(0.01, 0.01, 0.01));
    objectTransform->translate(vec3(-5, -3, 0));

    return object;
}

GameObject* BackpackDemo::createHelmet() {
    GameObject* object = loader->loadModel("models/helmet/source/helmet.fbx", "models/helmet/textures");

    Transform* objectTransform = object->transform;

    objectTransform->scaleBy(vec3(0.01, 0.01, 0.01));

    return object;
}

GameObject* BackpackDemo::createCube() {
    GameObject* cubeObject = loader->loadModel("models/cube/cube.obj");

    return cubeObject;
}

GameObject* BackpackDemo::createCurvedSurface() {
    const vector<float> controlPoints = {
        // row 1
        -1.5f, rand() % 3 - 1.0f, -1.5f,
        -0.5f, rand() % 3 - 1.0f, -1.5f,
        0.5f, rand() % 3 - 1.0f, -1.5f,
        1.5f, rand() % 3 - 1.0f, -1.5f,

        // row 2
        -1.5f, rand() % 3 - 1.0f, -0.5f,
        -0.5f, rand() % 3 - 1.0f, -0.5f,
        0.5f, rand() % 3 - 1.0f, -0.5f,
        1.5f, rand() % 3 - 1.0f, -0.5f,

        // row 3
        -1.5f, rand() % 3 - 1.0f, 0.5f,
        -0.5f, rand() % 3 - 1.0f, 0.5f,
        0.5f, rand() % 3 - 1.0f, 0.5f,
        1.5f, rand() % 3 - 1.0f, 0.5f,

        // row 4
        -1.5f, rand() % 3 - 1.0f, 1.5f,
        -0.5f, rand() % 3 - 1.0f, 1.5f,
        0.5f, rand() % 3 - 1.0f, 1.5f,
        1.5f, rand() % 3 - 1.0f, 1.5f,
    };

    GameObject* cubicPatchObject = Hierarchy::createGameObject();
    Transform* patchTransform = cubicPatchObject->transform;
    patchTransform->scaleBy(vec3(10, 10, 10));
    patchTransform->translate(vec3(0, -3, 0));

    const Texture* patchDiffuseTexture = loader->loadTexture("textures/metal_art_diffuse.jpg");
    const Texture* patchSpecularTexture = loader->loadTexture("textures/metal_art_specular.jpg");
    const Texture* patchNormalTexture = loader->loadTexture("textures/metal_art_normal.jpg");
    // TODO:
    const Texture* patchRoughnessTexture = loader->loadTexture("textures/metal_art_specular.jpg");
    const Texture* patchAOTexture = loader->loadTexture("textures/metal_art_specular.jpg");
    const Material patchMaterial(*patchSpecularTexture, *patchDiffuseTexture, *patchNormalTexture, *patchRoughnessTexture, *patchAOTexture);

    auto* cubicPatch = new CubicPatch(controlPoints, patchMaterial);
    Hierarchy::addComponent(cubicPatchObject, cubicPatch);

    return cubicPatchObject;
}

GameObject* BackpackDemo::createTerrain() {
    GameObject* terrainObject = Hierarchy::createGameObject();

    Texture* terrainTexture = loader->loadTexture("textures/iceland_heightmap.png", TERRAIN_OPTIONS);
    auto* terrain = new Terrain(terrainTexture, 20);

    Hierarchy::addComponent(terrainObject, terrain);

    return terrainObject;
}