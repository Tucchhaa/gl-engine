#include "gooch-shading-demo.hpp"

#include "../core/hierarchy.hpp"

GoochShadingDemo::GoochShadingDemo(Loader* loader): Scene(loader) {
    input = IInput::getInstance();
}

void GoochShadingDemo::setupScene() {
    createDragon();
    auto* cube = createCube();

    cube->transform->scaleBy(vec3(100, 1, 100));
    cube->transform->translate(vec3(0, 3.55, 0));

    // = light source =
    lights = Hierarchy::createGameObject();

    auto* mainLightSource = Hierarchy::createGameObject();

    Transform* lightTransform = mainLightSource->transform;
    lightTransform->translate(vec3(0, 20, 0));

    PointLight* pointLight0 = PointLight::D3250();
    Hierarchy::addComponent(mainLightSource, pointLight0);

    for(int i=0; i < 4; i++) {
        GameObject* gameObject = Hierarchy::createGameObject();
        PointLight* pointLight = PointLight::D100();

        float r = (float)rand()/RAND_MAX;
        float b = (float)rand()/RAND_MAX;
        float g = (float)rand()/RAND_MAX;

        pointLight->diffuse = vec3(r, g, b);
        pointLight->ambient = vec3(0, 0, 0);

        gameObject->transform->setPosition(vec3(0, 2.5, 0));

        lightTranslate.emplace_back(gameObject, vec3(r, 0, 1-b));

        Hierarchy::addComponent(gameObject, pointLight);
        Hierarchy::setParent(lights, gameObject);
    }

    Scene::setupScene();
}

void GoochShadingDemo::beforeRender() {
    for(int i=0; i < lightTranslate.size(); i++) {
        const GameObject* lightSource = lightTranslate[i].first;
        vec3 direction = lightTranslate[i].second;

        Transform* lightTransform = lightSource->transform;
        lightTransform->translate(direction * 10.0f * input->getDeltaTime());

        if(lightTransform->getPosition().x > 5 || lightTransform->getPosition().x < -5) {
            lightTranslate[i].second.x *= -1;
        }
        if(lightTransform->getPosition().z > 5 || lightTransform->getPosition().z < -5) {
            lightTranslate[i].second.z *= -1;
        }
    }

    Hierarchy::updateTransformTree(lights);

    Scene::beforeRender();
}

GameObject* GoochShadingDemo::createDragon() {
    GameObject* object = loader->loadModel("models/dragon/dragon.ply");

    object->transform->scaleBy(vec3(30, 30, 30));

    return object;
}

GameObject* GoochShadingDemo::createCube() {
    GameObject* cubeObject = loader->loadModel("models/cube/cube.obj");
    Transform* cubeTransform = cubeObject->transform;

    cubeTransform->translate(vec3(0, -3, 0));

    return cubeObject;
}