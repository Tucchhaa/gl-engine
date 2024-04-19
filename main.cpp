#include <iostream>
#include <ctime>

#include "apis/opengl/include.hpp"
#include "core/include.hpp"

using namespace std;

constexpr int SCREEN_WIDTH = 1280;
constexpr int SCREEN_HEIGHT = 720;

GameObject* createCurvedSurface(Loader* loader);
GameObject* createTerrain(Loader* loader);
GameObject* createBackpack(Loader* loader);
GameObject* createCube(Loader* loader);
GameObject* createCamera(Loader* loader);

void setupDefaultScene(Loader* loader);
void setupTunnelScene(Loader* loader);
void setupManyLightsScene(Loader* loader);

int backpackId;
vector<pair<GameObject*, vec3>> lightTranslate;

/*
TODO: optimizations
1) Skybox - no need to render it for pixel
2) Normal mapping - use tangent space
3) Light volumes
4) reconstruct position from depth
5) do not unbind textures in different shaders

Features:
1) Bloom
2) Scene events
3) Deferred + Forward renderings for transparency

Other:
rename shaders: fragment.frag -> scene.frag, screen-fragment.frag -> screen.frag
check for memory leaks
move game-object to /core
move #include to cpp files if possible
 */
int main() {
    IWindow* window = new Window();
    window->create(SCREEN_WIDTH, SCREEN_HEIGHT);

    auto* input = new Input(window);
    IResourceManager* resourceManager = &ResourceManager::getInstance();
    // IRenderer* renderer = new Renderer();
    IRenderer* renderer = new DeferredRenderer();

    Loader loader(resourceManager);
    Scene scene;
    Hierarchy::initialize();

    // ===

    GameObject* cameraObject = createCamera(&loader);

    Camera* camera = Hierarchy::Components<Camera>::get(cameraObject);
    Transform* cameraTransform = cameraObject->transform;

    // ===

    // setupDefaultScene(&loader);
    // setupTunnelScene(&loader);
    setupManyLightsScene(&loader);

    Transform* backpackTransform = Hierarchy::getGameObject(backpackId)->transform;

    // ===

    Hierarchy::updateTransformTree();
    scene.setCamera(camera);
    scene.processHierarchy();
    renderer->setScene(&scene);

    float speed = 15.0f;
    float rotationSpeed = 1.0f;
    int frameCnt = 0;
    time_t lastTime = time(nullptr);

    while (window->isOpen())
    {
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
            quat horizontalRotation = quat(vec3(0, -input->axisHorizontal() * rotationSpeed * input->getDeltaTime(), 0));
            quat verticalRotation   = quat(vec3(-input->axisVertical() * rotationSpeed * input->getDeltaTime(), 0, 0));

            cameraTransform->rotate(horizontalRotation, &Transform::World);
            cameraTransform->rotate(verticalRotation);
        }
        else {
            cameraTransform->translate(input->axisVec3() * speed * input->getDeltaTime());
        }

        // backpackTransform->rotate(quat(vec3(0, radians(0.05f), 0)));
        // Hierarchy::updateTransformTree(backpackTransform);

        for(int i=0; i < lightTranslate.size(); i++) {
            GameObject* lightSource = lightTranslate[i].first;
            vec3 direction = lightTranslate[i].second;

            Transform* lightTransform = lightSource->transform;
            lightTransform->translate(direction * 10.0f * input->getDeltaTime());

            if(lightTransform->getPosition().x > 80 || lightTransform->getPosition().x < -80) {
                lightTranslate[i].second.x *= -1;
            }
            if(lightTransform->getPosition().z > 80 || lightTransform->getPosition().z < -80) {
                lightTranslate[i].second.z *= -1;
            }
        }

        Hierarchy::updateTransformTree();

        renderer->render();

        window->onRendered();
    }

    window->terminate();

    return 0;
}

void setupDefaultScene(Loader* loader) {
    // GameObject* terrainObject = createTerrain(&loader);
    // GameObject* cubicPatchObject = createCurvedSurface(&loader);
    GameObject* backpackObject = createBackpack(loader);
    GameObject* cubeObject = createCube(loader);

    backpackId = backpackObject->ID;

    cubeObject->transform->scaleBy(vec3(100, 1, 100));

    // = light source =
    auto* lightSource = Hierarchy::createGameObject();

    Transform* lightTransform = Hierarchy::getTransform(lightSource);
    lightTransform->translate(vec3(0, 20, 0));
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
}

void setupTunnelScene(Loader* loader) {
    GameObject* topCube = createCube(loader);
    GameObject* rightCube = createCube(loader);
    GameObject* bottomCube = createCube(loader);
    GameObject* leftCube = createCube(loader);
    GameObject* backCube = createCube(loader);
    GameObject* cube = createCube(loader);

    topCube->transform->scaleBy(vec3(1, 1, 100));
    rightCube->transform->scaleBy(vec3(1, 1, 100));
    bottomCube->transform->scaleBy(vec3(1, 1, 100));
    leftCube->transform->scaleBy(vec3(1, 1, 100));
    cube->transform->scaleBy(vec3(0.1, 0.1, 0.1));

    topCube->transform->translate(vec3(0, 1.9, 0));
    rightCube->transform->translate(vec3(1.9, 0, 0));
    bottomCube->transform->translate(vec3(0, -1.9, 0));
    leftCube->transform->translate(vec3(-1.9, 0, 0));
    backCube->transform->translate(vec3(0, 0, 5));

    // TODO: rotate cubes because of the bug
    // leftCube->transform->rotate(vec3(0, 0, radians(180.0f)));
    // bottomCube->transform->rotate(vec3(0, 0, radians(180.0f)));

    // = light source =
    auto* lightSource = Hierarchy::createGameObject();
    auto* pointLight = PointLight::D3250();
    auto* pointLight3 = PointLight::D3250();
    auto* pointLight4 = PointLight::D3250();
    // pointLight->diffuse = vec3(4, 4, 4);
    // pointLight->quadratic = 0.000007;
    lightSource->transform->translate(vec3(0, -3, 0));
    cube->transform->setPosition(lightSource->transform->getPosition());

    auto* redLightSource = Hierarchy::createGameObject();
    auto* pointLight1 = PointLight::D3250();
    pointLight1->diffuse = vec3(0.3, 0, 0);
    redLightSource->transform->translate(vec3(0, -3, -20));

    auto* blueLightSource = Hierarchy::createGameObject();
    auto* pointLight2 = PointLight::D6();
    pointLight2->diffuse = vec3(0, 0, 0.3);
    blueLightSource->transform->translate(vec3(0, -3.5, -25));

    Hierarchy::addComponent(lightSource, pointLight);
    Hierarchy::addComponent(lightSource, pointLight3);
    Hierarchy::addComponent(lightSource, pointLight4);

    Hierarchy::addComponent(redLightSource, pointLight1);
    Hierarchy::addComponent(blueLightSource, pointLight2);
}

void setupManyLightsScene(Loader* loader) {
    srand(time(nullptr));

    const int N = 20;
    const int LIGHTS_NUM = 200;

    float distance = 4.0f;

    for(int i=-N; i < N; i++) {
        for(int j=-N; j < N; j++) {
            GameObject* cube = createCube(loader);

            cube->transform->setPosition(vec3(distance * i, 0, distance * j));
        }
    }

    GameObject* lights = Hierarchy::createGameObject();

    // Hierarchy::addComponent(lights, PointLight::D3250());

    for(int i=0; i < LIGHTS_NUM; i++) {
        GameObject* lightSource = Hierarchy::createGameObject();
        PointLight* pointLight = PointLight::D50();

        float x = N * 2.5f * ((float)rand()/RAND_MAX * 2 - 1);
        float z = N * 2.5f * ((float)rand()/RAND_MAX * 2 - 1);

        float r = (float)rand()/RAND_MAX;
        float b = (float)rand()/RAND_MAX;
        float g = (float)rand()/RAND_MAX;

        // cout << x << " " << z << endl;

        pointLight->diffuse = vec3(r, g, b);
        pointLight->ambient = vec3(0, 0, 0);

        lightSource->transform->setPosition(vec3(x, 2.5, z));

        lightTranslate.emplace_back(lightSource, vec3(r, 0, 1-b));

        Hierarchy::addComponent(lightSource, pointLight);
        Hierarchy::setParent(lights, lightSource);
    }
}

// ===

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
    const Texture* patchNormalTexture = loader->loadTexture("textures/metal_art_normal.jpg");
    const Material patchMaterial(*patchSpecularTexture, *patchDiffuseTexture, *patchNormalTexture);

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