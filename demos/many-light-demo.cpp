#include "many-light-demo.hpp"

#include <ctime>
#include "../core/hierarchy.hpp"

ManyLightsDemo::ManyLightsDemo(Loader* loader): Scene(loader) {
    input = IInput::getInstance();
}

void ManyLightsDemo::setupScene() {
    srand(time(nullptr));

    const int N = 20;
    const int LIGHTS_NUM = 2000;

    float distance = 4.0f;

    for(int i=-N; i < N; i++) {
        for(int j=-N; j < N; j++) {
            GameObject* cube = createCube();

            cube->transform->setPosition(vec3(distance * i, 0, distance * j));
        }
    }

    GameObject* lights = Hierarchy::createGameObject();

    // Hierarchy::addComponent(lights, PointLight::D3250());

    for(int i=0; i < LIGHTS_NUM; i++) {
        GameObject* lightSource = Hierarchy::createGameObject();
        PointLight* pointLight = PointLight::D20();

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

    Scene::setupScene();
}

void ManyLightsDemo::beforeRender() {
    for(int i=0; i < lightTranslate.size(); i++) {
        const GameObject* lightSource = lightTranslate[i].first;
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

    Scene::beforeRender();
}

GameObject* ManyLightsDemo::createCube() {
    GameObject* cubeObject = loader->loadModel("models/cube/cube.obj");
    Transform* cubeTransform = cubeObject->transform;

    cubeTransform->translate(vec3(0, -3, 0));

    return cubeObject;
}