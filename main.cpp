#include <iostream>
#include <ctime>

#include "apis/opengl/include.hpp"
#include "core/include.hpp"
#include "demos/backpack-demo.hpp"
#include "demos/many-light-demo.hpp"
#include "demos/tunnel-demo.hpp"

using namespace std;

constexpr int SCREEN_WIDTH = 1280;
constexpr int SCREEN_HEIGHT = 720;

GameObject* createCamera(Loader* loader);

void setupTunnelScene(Loader* loader);
void setupManyLightsScene(Loader* loader);

int backpackId;
vector<pair<GameObject*, vec3>> lightTranslate;

/*
TODO:
Optimizations:
1) Normal mapping - use tangent space
2) reconstruct position from depth
3) do not unbind textures in different shaders
4) render objects with same material in one batch
5) instancing for meshes
6) instancing for light volumes
7) use stencil pass for light volumes: https://ogldev.org/www/tutorial37/tutorial37.html

Features:
1) Bloom
2) Scene events
3) Deferred + Forward renderings for transparency
4) PBR

Other:
Class with pre-defined meshes: cube, sphere, plane, etc.
check for memory leaks
 */
int main() {
    IWindow* window = new Window();
    window->create(SCREEN_WIDTH, SCREEN_HEIGHT);

    IInput* input = new Input(window);
    IResourceManager* resourceManager = &ResourceManager::getInstance();
    IRenderer* renderer = new DeferredRenderer();
    Loader loader(resourceManager);

    Hierarchy::initialize();

    // Scene scene(&loader);
    BackpackDemo scene(&loader);
    // TunnelDemo scene(&loader);
    // ManyLightsDemo scene(&loader);

    // ===

    GameObject* cameraObject = createCamera(&loader);

    Camera* camera = Hierarchy::Components<Camera>::get(cameraObject);
    Transform* cameraTransform = cameraObject->transform;

    // ===

    scene.setCamera(camera);
    scene.setupScene();
    renderer->setScene(&scene);
    input->process();

    int frameCnt = 0;
    time_t lastTime = time(nullptr);

    while (window->isOpen()) {
        frameCnt++;

        time_t now = time(nullptr);
        time_t diff = difftime(now, lastTime);
        if(diff >= 1) {
            cout << "FPS: " << frameCnt << endl;
            frameCnt = 0;
            lastTime = now;
        }

        // = Input =
        input->process();

        if(input->isShiftPressed()) {
            float rotationSpeed = 1.0f;

            quat horizontalRotation = quat(vec3(0, -input->axisHorizontal() * rotationSpeed * input->getDeltaTime(), 0));
            quat verticalRotation   = quat(vec3(-input->axisVertical() * rotationSpeed * input->getDeltaTime(), 0, 0));

            cameraTransform->rotate(horizontalRotation, &Transform::World);
            cameraTransform->rotate(verticalRotation);
        }
        else {
            float speed = 15.0f;

            cameraTransform->translate(input->axisVec3() * speed * input->getDeltaTime());
        }

        renderer->render();

        window->onRendered();
    }

    scene.finish();
    window->terminate();

    return 0;
}

GameObject* createCamera(Loader* loader) {
    auto* cameraObject = Hierarchy::createGameObject();
    Transform* cameraTransform = cameraObject->transform;

    auto* camera = new Camera(radians(45.0f), 0.1f, 3000.0f);
    camera->cubeMap = loader->loadCubeMap("textures/skybox");
    Hierarchy::addComponent(cameraObject, camera);

    cameraTransform->translate(vec3(-10, 25, -30));
    cameraTransform->rotate(quat(vec3(0, radians(-15.0), 0)));

    camera->setScreenSizes((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT);

    return cameraObject;
}