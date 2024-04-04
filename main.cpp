/**
 * Author: Eldar Iusupzhanov
 * Student ID: 112550085
 * GitHub repo: https://github.com/Tucchhaa/gl-engine
 * Demo branch: https://github.com/Tucchhaa/gl-engine/tree/curved_surface_demo
 */
#include <iostream>

#include <cmath>
#include <cstdlib>
#include <ctime>

#include "apis/opengl/include.hpp"
#include "core/include.hpp"

using namespace std;

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 800;

int main() {
    srand(time(0));

    vector<float> controlPoints = {
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

    IWindow* window = new Window();
    window->create(SCREEN_WIDTH, SCREEN_HEIGHT);

    auto* input = new Input(window);
    IResourceManager* resourceManager = &ResourceManager::getInstance();
    auto* renderer = new Renderer();

    Loader loader(resourceManager);
    Scene scene;
    Hierarchy::initialize();

    // = cubic patch =
    GameObject* cubicPatchObject = Hierarchy::createGameObject();
    Transform* patchTransform = Hierarchy::getTransform(cubicPatchObject);

    Texture* patchDiffuseTexture = loader.loadTexture("textures/metal_art_diffuse.jpg");
    Texture* patchSpecularTexture = loader.loadTexture("textures/metal_art_specular.jpg");
    Material patchMaterial(*patchSpecularTexture, *patchDiffuseTexture);

    CubicPatch cubicPatch(controlPoints, patchMaterial);
    Hierarchy::addComponent(cubicPatchObject, &cubicPatch);

    // = camera =
    auto* cameraObject = Hierarchy::createGameObject();

    Transform* cameraTransform = Hierarchy::getTransform(cameraObject);
    cameraTransform->translate(vec3(0, 4, -7));
    cameraTransform->rotate(quat(vec3(-0.45f, 0, 0)));

    auto* camera = new Camera(radians(45.0f), 0.1f, 3000.0f);
    camera->cubeMap = loader.loadCubeMap("textures/skybox");
    Hierarchy::addComponent(cameraObject, camera);

    camera->setScreenSizes((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT);

    // = light source =
    auto* lightSource = Hierarchy::createGameObject();

    Transform* lightTransform = Hierarchy::getTransform(lightSource);
    lightTransform->setValues(vec3(0, 5, 0), quat(vec3(0, radians(180.0), 0)));

//    auto* directLight0 = new DirectLight();
    PointLight* pointLight0 = PointLight::D100();

//    Hierarchy::addComponent(lightSource, directLight0);
    Hierarchy::addComponent(lightSource, pointLight0);

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

        if(input->isSpacePressed) {
            renderer->isPolygonLineMode = !renderer->isPolygonLineMode;
        }

        if(input->isQPressed) {
            if(input->isShiftPressed()) {
                cubicPatch.tessOuterLevel = std::max(1.0f, cubicPatch.tessOuterLevel - 3.0f);
            }
            else {
                cubicPatch.tessInnerLevel = std::max(1.0f, cubicPatch.tessInnerLevel - 3.0f);
            }
        }
        if(input->isEPressed) {
            if(input->isShiftPressed()) {
                cubicPatch.tessOuterLevel = std::min(60.0f, cubicPatch.tessOuterLevel + 3.0f);
            }
            else {
                cubicPatch.tessInnerLevel = std::min(60.0f, cubicPatch.tessInnerLevel + 3.0f);
            }
        }

        patchTransform->rotate(quat(vec3(0, radians(0.05f), 0)));

        Hierarchy::updateTransformTree(patchTransform);
        Hierarchy::updateTransformTree(cameraTransform);

        renderer->render();

        window->onRendered();
    }

    window->terminate();

    return 0;
}
