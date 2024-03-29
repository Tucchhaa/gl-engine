#include <iostream>

#include "apis/opengl/include.hpp"
#include "core/include.hpp"

using namespace std;

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 800;

int main() {
    IWindow* window = new Window();
    window->create(SCREEN_WIDTH, SCREEN_HEIGHT);

    IInput* input = new Input(window);
    IResourceManager* resourceManager = &ResourceManager::getInstance();
    IRenderer* renderer = new Renderer();

    Loader loader(resourceManager);
    Scene scene;

    // = backpack model =
    GameObject* object = loader.loadModel("models/backpack/backpack.obj");

    Hierarchy::addGameObject(object);
    Transform* objectTransform = Hierarchy::getTransform(object);
    objectTransform->translate(vec3(0, 0, 10));
    objectTransform->scaleBy(vec3(10, 10, 10));
    Hierarchy::updateTransformTree(objectTransform);

    // = camera =
    auto* cameraObject = new GameObject();
    Hierarchy::addGameObject(cameraObject);
    Transform* cameraTransform = Hierarchy::getTransform(cameraObject);
    cameraTransform->translate(vec3(0, 0, -20));
//    cameraTransform->rotate(vec3(0, 3.141592, 0));

    auto* camera = new Camera(radians(45.0f), 0.1f, 3000.0f);
    camera->cubeMap = loader.loadCubeMap("textures/skybox");
    Hierarchy::addComponent(cameraObject, camera);

    camera->setScreenSizes((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT);

    // = light source =
    auto* lightSource = new GameObject();
    Hierarchy::addGameObject(lightSource);

    Hierarchy::getTransform(lightSource)->setValues(vec3(-10, 20, -50), quat(vec3(0, radians(180.0), 0)));

    auto* directLight0 = new DirectLight();
    PointLight* pointLight0 = PointLight::D3250();

    Hierarchy::addComponent(lightSource, directLight0);
    Hierarchy::addComponent(lightSource, pointLight0);

    cameraTransform->rotate(quat(vec3(0, 0.1, 0)));

    // = Flashlight =
    auto* flashlight = new GameObject();
    Hierarchy::addGameObject(flashlight);

    auto* spotLight0 = SpotLight::D3250(radians(15.0));
    Hierarchy::addComponent(flashlight, spotLight0);

    Hierarchy::setParent(cameraObject, flashlight);
    Hierarchy::updateTransformTree(cameraTransform);

    // ===

    scene.setCamera(camera);
    scene.processHierarchy();
    renderer->setScene(&scene);

    float speed = 150.0f;
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
        Hierarchy::updateTransformTree(cameraTransform);

//        objectTransform->rotate(quat(vec3(0, radians(0.15f), 0)));
//        Hierarchy::updateTransformTree(objectTransform);

        renderer->render();

        window->onRendered();
    }

    window->terminate();

    return 0;
}
