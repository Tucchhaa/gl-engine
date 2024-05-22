#include "parallax-demo.hpp"

#include "../apis/base/iengine.hpp"
#include "../core/hierarchy.hpp"

void ParallaxDemo::setup() {
    auto* plane = createPlane();
    plane->transform->scaleBy(5., 1., 5.);
    Hierarchy::addToHierarchy(plane);

    light = Hierarchy::createGameObjectInTree();
    auto* pointLight = PointLight::D100();
    light->components.add(pointLight);
    light->transform->translate(vec3(0, 2, 1));

    auto* material = plane->getDataItem<Material>();
    material->diffuseTexture = *loader->loadTexture("textures/bricks.jpg");
    material->normalTexture = *loader->loadTexture("textures/bricks_normal.jpg");
    material->heightTexture = *loader->loadTexture("textures/bricks_disp.jpg");

    Scene::setup();
}

void ParallaxDemo::beforeRender() {
    pointLightController();
    heightScaleController();

    Scene::beforeRender();
}

void ParallaxDemo::pointLightController() {
    IInput* input = IEngine::Input;
    Transform* transform = light->transform;

    if(input->isShiftPressed())
        return;

    float speed = 15.0f;

    vec3 direction = vec3(0);

    if(input->isArrowUpPressed())
        direction.z = 1;
    else if(input->isArrowDownPressed())
        direction.z = -1;

    if(input->isArrowRightPressed())
        direction.x = 1;
    else if(input->isArrowLeftPressed())
        direction.x = -1;

    transform->translate(direction * speed * input->getDeltaTime());
}

void ParallaxDemo::heightScaleController() {
    IInput* input = IEngine::Input;
    const float delta = 0.001f;

    if(input->isShiftPressed()) {
        if(input->isArrowUpPressed())
            heightScale += delta;
        if(input->isArrowDownPressed())
            heightScale -= delta;
    }
}
