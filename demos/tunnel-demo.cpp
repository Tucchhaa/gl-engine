#include "tunnel-demo.hpp"

#include "../core/hierarchy.hpp"

void TunnelDemo::setup() {
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

    Hierarchy::addToHierarchy(topCube);
    Hierarchy::addToHierarchy(rightCube);
    Hierarchy::addToHierarchy(bottomCube);
    Hierarchy::addToHierarchy(leftCube);
    Hierarchy::addToHierarchy(backCube);
    Hierarchy::addToHierarchy(cube);

    // = light source =
    auto* lightSource0 = Hierarchy::createGameObjectInTree();
    auto* pointLight0 = PointLight::D3250();
    lightSource0->transform->translate(vec3(0, 30, 0));
    lightSource0->components.add(pointLight0);

    auto* lightSource = Hierarchy::createGameObjectInTree();
    auto* pointLight = PointLight::D3250();

    // pointLight->diffuse = vec3(4, 4, 4);
    // pointLight->quadratic = 0.000007;
    cube->transform->setPosition(lightSource->transform->getPosition());

    auto* redLightSource = Hierarchy::createGameObjectInTree();
    auto* pointLight1 = PointLight::D3250();
    pointLight1->diffuse = vec3(0.3, 0, 0);
    redLightSource->transform->translate(vec3(0, -2, -20));

    auto* blueLightSource = Hierarchy::createGameObjectInTree();
    auto* pointLight2 = PointLight::D6();
    pointLight2->diffuse = vec3(0, 0, 0.3);
    blueLightSource->transform->translate(vec3(0, -2.5, -25));

    lightSource->components.add(pointLight);

    redLightSource->components.add(pointLight1);
    blueLightSource->components.add(pointLight2);

    Scene::setup();
}

void TunnelDemo::beforeRender() {
    Scene::beforeRender();
}
