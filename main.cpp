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

GameObject* createCamera();

/*
TODO:
Optimizations:
1) Normal mapping - use tangent space
2) do not unbind textures in different shaders
3) render objects with same material in one batch
4) instancing for meshes
5) instancing for light volumes
6) use stencil pass for light volumes: https://ogldev.org/www/tutorial37/tutorial37.html
7) use uniform buffer objects

Features:
1) Bloom
2) Deferred + Forward renderings for transparency
3) PBR
4) Physics
5) Compatibility with Windows

Other:
check for memory leaks
 */

Loader* loader;
IWindow* window;

int main() {
    IEngine* engine = new GlEngine();
    window = IEngine::Window;
    IInput* input = IEngine::Input;
    IRenderer* renderer = IEngine::Renderer;
    loader = IEngine::Loader;

    // Scene scene;
    BackpackDemo scene;
    // TunnelDemo scene;
    // ManyLightsDemo scene;

    // ===

    GameObject* cameraObject = createCamera();

    auto* camera = cameraObject->components.get<Camera>();
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

GameObject* createCamera() {
    auto* cameraObject = Hierarchy::addGameObject();
    Transform* cameraTransform = cameraObject->transform;

    auto* camera = new Camera(radians(45.0f), 0.1f, 3000.0f);
    camera->cubeMap = loader->loadCubeMap("textures/skybox");
    cameraObject->components.add(camera);

    cameraTransform->translate(vec3(-10, 25, -30));
    cameraTransform->rotate(quat(vec3(0, radians(-15.0), 0)));

    camera->setScreenSizes(window->screenWidth, window->screenHeight);

    return cameraObject;
}