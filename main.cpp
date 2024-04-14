#include <iostream>

#include "apis/opengl/include.hpp"
#include "core/include.hpp"

using namespace std;

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 800;

GameObject* createCurvedSurface(Loader* loader);
GameObject* createTerrain(Loader* loader);
GameObject* createBackpack(Loader* loader);
GameObject* createCube(Loader* loader);
GameObject* createCamera(Loader* loader);

int main() {
    IWindow* window = new Window();
    window->create(SCREEN_WIDTH, SCREEN_HEIGHT);

    auto* input = new Input(window);
    IResourceManager* resourceManager = &ResourceManager::getInstance();
    IRenderer* renderer = new Renderer();

    Loader loader(resourceManager);
    Scene scene;
    Hierarchy::initialize();

    GameObject* terrainObject = createTerrain(&loader);
    GameObject* cubicPatchObject = createCurvedSurface(&loader);
    GameObject* backpackObject = createBackpack(&loader);
    GameObject* cubeObject = createCube(&loader);
    GameObject* cameraObject = createCamera(&loader);

    Camera* camera = Hierarchy::Components<Camera>::get(cameraObject);

    Transform* backpackTransform = backpackObject->transform;
    Transform* cameraTransform = cameraObject->transform;

    // = light source =
    auto* lightSource = Hierarchy::createGameObject();

    Transform* lightTransform = Hierarchy::getTransform(lightSource);
    lightTransform->translate(vec3(-10, 20, -50));
    lightTransform->rotate(vec3(0, radians(180.0), 0));
    lightTransform->rotate(vec3(radians(-30.0), 0, 0));

    auto* directLight0 = new DirectLight();
    PointLight* pointLight0 = PointLight::D3250();

    Hierarchy::addComponent(lightSource, directLight0);
//    Hierarchy::addComponent(lightSource, pointLight0);

    // = Flashlight =
//    auto* flashlight = Hierarchy::createGameObject();
//
//    auto* spotLight0 = SpotLight::D3250(radians(10.0));
//    Hierarchy::addComponent(flashlight, spotLight0);
//
//    Hierarchy::setParent(cameraObject, flashlight);

    // ===
    Hierarchy::updateTransformTree();
    scene.setCamera(camera);
    scene.processHierarchy();
    renderer->setScene(&scene);

    float speed = 15.0f;
    float rotationSpeed = 1.0f;

    while (window->isOpen())
    {
        // = Input =
        input->process();

        if(input->isShiftPressed()) {
            quat horizontalRotation = quat(vec3(0, -input->axisHorizontal() * rotationSpeed * input->getDeltaTime(), 0));
            quat verticalRotation   = quat(vec3(-input->axisVertical() * rotationSpeed * input->getDeltaTime(), 0, 0));

            cameraTransform->rotate(horizontalRotation, &Transform::World);
            cameraTransform->rotate(verticalRotation);
        }
        else {
            cameraTransform->translate(input->axisVec3() * speed * input->getDeltaTime());
        }

        backpackTransform->rotate(quat(vec3(0, radians(0.15f), 0)));
        Hierarchy::updateTransformTree(backpackTransform);

        Hierarchy::updateTransformTree(cameraTransform);

        renderer->render();

        window->onRendered();
    }

    window->terminate();

    return 0;
}

GameObject* createCurvedSurface(Loader* loader) {
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
    Transform* patchTransform = Hierarchy::getTransform(cubicPatchObject);
    patchTransform->scaleBy(vec3(10, 10, 10));
    patchTransform->translate(vec3(0, -3, 0));

    const Texture* patchDiffuseTexture = loader->loadTexture("textures/metal_art_diffuse.jpg");
    const Texture* patchSpecularTexture = loader->loadTexture("textures/metal_art_specular.jpg");
    const Material patchMaterial(*patchSpecularTexture, *patchDiffuseTexture);

    auto* cubicPatch = new CubicPatch(controlPoints, patchMaterial);
    Hierarchy::addComponent(cubicPatchObject, cubicPatch);

    return cubicPatchObject;
}

GameObject* createTerrain(Loader* loader) {
    GameObject* terrainObject = Hierarchy::createGameObject();

    Texture* terrainTexture = loader->loadTexture("textures/iceland_heightmap.png", TERRAIN_OPTIONS);
    auto* terrain = new Terrain(terrainTexture, 20);

    Hierarchy::addComponent(terrainObject, terrain);

    return terrainObject;
}

GameObject* createBackpack(Loader* loader) {
    GameObject* object = loader->loadModel("models/backpack/backpack.obj");

    Transform* objectTransform = Hierarchy::getTransform(object);
    objectTransform->translate(vec3(0, 0, -5));
    // objectTransform->scaleBy(vec3(5, 5, 5));

    return object;
}

GameObject* createCube(Loader* loader) {
    GameObject* cubeObject = loader->loadModel("models/cube/cube.obj");
    Transform* cubeTransform = Hierarchy::getTransform(cubeObject);

    cubeTransform->translate(vec3(0, -3, 0));
    cubeTransform->scaleBy(vec3(100, 1, 100));

    return cubeObject;
}

GameObject* createCamera(Loader* loader) {
    auto* cameraObject = Hierarchy::createGameObject();
    Transform* cameraTransform = Hierarchy::getTransform(cameraObject);

    auto* camera = new Camera(radians(45.0f), 0.1f, 3000.0f);
    camera->cubeMap = loader->loadCubeMap("textures/skybox");
    Hierarchy::addComponent(cameraObject, camera);

    cameraTransform->translate(vec3(-10, 25, -30));
    cameraTransform->rotate(quat(vec3(0, radians(-15.0), 0)));

    camera->setScreenSizes((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT);

    return cameraObject;
}