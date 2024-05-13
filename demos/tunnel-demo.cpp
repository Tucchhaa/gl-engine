#include "tunnel-demo.hpp"

#include "../core/hierarchy.hpp"

void TunnelDemo::setupScene() {
    GameObject* topCube = createCube();
    GameObject* rightCube = createCube();
    GameObject* bottomCube = createCube();
    GameObject* leftCube = createCube();
    GameObject* backCube = createCube();
    GameObject* cube = createCube();

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
    leftCube->transform->rotate(vec3(0, 0, radians(180.0f)));
    bottomCube->transform->rotate(vec3(0, 0, radians(180.0f)));

    // = light source =
    auto* lightSource = Hierarchy::addGameObject();
    auto* pointLight = PointLight::D3250();
    auto* pointLight3 = PointLight::D3250();
    auto* pointLight4 = PointLight::D3250();
    // pointLight->diffuse = vec3(4, 4, 4);
    // pointLight->quadratic = 0.000007;
    cube->transform->setPosition(lightSource->transform->getPosition());

    auto* redLightSource = Hierarchy::addGameObject();
    auto* pointLight1 = PointLight::D3250();
    pointLight1->diffuse = vec3(0.3, 0, 0);
    redLightSource->transform->translate(vec3(0, -3, -20));

    auto* blueLightSource = Hierarchy::addGameObject();
    auto* pointLight2 = PointLight::D6();
    pointLight2->diffuse = vec3(0, 0, 0.3);
    blueLightSource->transform->translate(vec3(0, -3.5, -25));

    lightSource->components.add(pointLight);
    lightSource->components.add(pointLight3);
    lightSource->components.add(pointLight4);

    lightSource->components.add(pointLight1);
    lightSource->components.add(pointLight2);

    Scene::setupScene();
}

void TunnelDemo::beforeRender() {
    Scene::beforeRender();
}
