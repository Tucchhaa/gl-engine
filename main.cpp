#include <iostream>

#include "apis/opengl/include.hpp"
#include "core/include.hpp"

using namespace std;

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 800;

int main() {
    IWindow* window = new Window();
    window->create(SCREEN_WIDTH, SCREEN_HEIGHT);

    auto* input = new Input(window);
    IResourceManager* resourceManager = &ResourceManager::getInstance();
    IRenderer* renderer = new Renderer();

    Loader loader(resourceManager);
    Scene scene;
    Hierarchy::initialize();

    // = terrain =
    auto* terrainObject = new GameObject();
    Hierarchy::addGameObject(terrainObject);

    Texture* terrainTexture = loader.loadTexture("textures/iceland_heightmap.png", TERRAIN_OPTIONS);
    Terrain terrain(terrainTexture, 20);

    Hierarchy::addComponent(terrainObject, &terrain);

    // = cubic patch =
    auto* cubicPatchObject = new GameObject();
    Hierarchy::addGameObject(cubicPatchObject);
    Transform* patchTransform = Hierarchy::getTransform(cubicPatchObject);

    Texture* patchDiffuseTexture = loader.loadTexture("textures/metal_art_diffuse.jpg");
    Texture* patchSpecularTexture = loader.loadTexture("textures/metal_art_specular.jpg");
    Material patchMaterial(*patchSpecularTexture, *patchDiffuseTexture);

    CubicPatch cubicPatch(patchMaterial);
    Hierarchy::addComponent(cubicPatchObject, &cubicPatch);
//    Hierarchy::updateTransformTree(patchTransform);

    // = backpack model =
    GameObject* object = loader.loadModel("models/backpack/backpack.obj");

    Hierarchy::addGameObject(object);
    Transform* objectTransform = Hierarchy::getTransform(object);
    objectTransform->translate(vec3(0, 25, 10));
    objectTransform->scaleBy(vec3(5, 5, 5));
//    Hierarchy::updateTransformTree(objectTransform);

    // = camera =
    auto* cameraObject = new GameObject();
    Hierarchy::addGameObject(cameraObject);
    Transform* cameraTransform = Hierarchy::getTransform(cameraObject);

    auto* camera = new Camera(radians(45.0f), 0.1f, 3000.0f);
    camera->cubeMap = loader.loadCubeMap("textures/skybox");
    Hierarchy::addComponent(cameraObject, camera);

    camera->setScreenSizes((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT);

    // = light source =
    auto* lightSource = new GameObject();
    Hierarchy::addGameObject(lightSource);

    Transform* lightTransform = Hierarchy::getTransform(lightSource);
    lightTransform->setValues(vec3(-10, 20, -50), quat(vec3(0, radians(180.0), 0)));

    auto* directLight0 = new DirectLight();
    PointLight* pointLight0 = PointLight::D3250();

    Hierarchy::addComponent(lightSource, directLight0);
    Hierarchy::addComponent(lightSource, pointLight0);

    // = Flashlight =
    auto* flashlight = new GameObject();
    Hierarchy::addGameObject(flashlight);

    auto* spotLight0 = SpotLight::D3250(radians(10.0));
    Hierarchy::addComponent(flashlight, spotLight0);

    Hierarchy::setParent(cameraObject, flashlight);
//    Hierarchy::updateTransformTree(cameraTransform);

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

            cameraTransform->rotate(horizontalRotation, Transform::World);
            cameraTransform->rotate(verticalRotation);
        }
        else {
            cameraTransform->translate(input->axisVec3() * speed * input->getDeltaTime());
        }

        patchTransform->rotate(quat(vec3(0, radians(0.15f), 0)));
        Hierarchy::updateTransformTree(patchTransform);

        Hierarchy::updateTransformTree(cameraTransform);

        renderer->render();

        window->onRendered();
    }

    window->terminate();

    return 0;
}
