#include "collision-demo.hpp"

#include "../apis/base/iengine.hpp"
#include "../core/hierarchy.hpp"
#include "../core/components/colliders/sphere-collider.hpp"
#include "../core/components/colliders/box-collider.hpp"

void CollisionDemo::setup() {
    GameObject* ground = createCube();
    ground->transform->scaleBy(100, 1, 100);
    Hierarchy::addToHierarchy(ground);
    auto* groundCollider = new BoxCollider(100, 1, 100);
    groundCollider->setMass(0.);
    ground->components.add(groundCollider);

    auto light = Hierarchy::createGameObjectInTree();
    auto pointLight = PointLight::D3250();
    light->components.add(pointLight);
    light->transform->translate(vec3(0, 20, 0));

    const int N = 5;
    const float distance = 2;
    float bias = static_cast<float>(N) / 2 * distance;
    int verticalBias = 20;

    for(int i=0; i < N; i++) {
        for(int j=0; j < N; j++) {
            for(int k=0; k < N; k++) {
                GameObject* sphere = createSphere();

                sphere->transform->translate(vec3(i * distance - bias, j * distance + verticalBias, k * distance - bias));

                auto* collider = new SphereCollider(1.);

                sphere->components.add(collider);

                Hierarchy::addToHierarchy(sphere);
            }
        }
    }

    Scene::setup();
}

void CollisionDemo::beforeRender() {
    IEngine::PhysicsEngine->dynamicsWorld->stepSimulation(IEngine::Input->getDeltaTime());

    Scene::beforeRender();
}