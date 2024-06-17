#include "fps-demo.hpp"

#include <iostream>

#include "../apis/base/iengine.hpp"
#include "../apis/opengl/input.hpp"
#include "../core/hierarchy.hpp"
#include "../core/components/colliders/box-collider.hpp"
#include "../core/components/colliders/sphere-collider.hpp"

float getRandom(int mn, int mx) {
    const int range = mx - mn + 1;

    return rand() % range + mn;
}

bool doSquaresOverlap(float ax, float ay, float aside, float bx, float by, float bside) {
    // Calculate the left and rightmost X coordinates of each square
    double left1 = ax - aside / 2;
    double right1 = ax + aside / 2;
    double left2 = bx - bside / 2;
    double right2 = bx + bside / 2;

    // Check if the X intervals overlap
    if (left1 >= right2 || right1 <= left2) {
        return false;
    }

    // Calculate the top and bottom Y coordinates of each square
    double top1 = ay + aside / 2;
    double bottom1 = ay - aside / 2;
    double top2 = by + bside / 2;
    double bottom2 = by - bside / 2;

    // Check if the Y intervals overlap
    if (top1 <= bottom2 || bottom1 >= top2) {
        return false;
    }

    // If both X and Y intervals overlap, the squares overlap
    return true;
}

void FpsDemo::setup() {
    srand(time(nullptr));

    // = ground =
    GameObject* plane = createPlane();
    auto* planeMaterial = new Material();

    plane->transform->scaleBy(vec3(100, 1, 100));
    plane->transform->translate(vec3(0, 0, 0));

    planeMaterial->diffuseTexture = *loader->loadTexture("textures/hexagon/albedo.png");
    planeMaterial->normalTexture = *loader->loadTexture("textures/hexagon/normal.png");
    planeMaterial->specularTexture = *loader->loadTexture("textures/hexagon/metallic.png");
    planeMaterial->roughnessTexture = *loader->loadTexture("textures/hexagon/roughness.png");
    planeMaterial->aoTexture = *loader->loadTexture("textures/hexagon/ao.png");

    auto* mesh = plane->components.getAllFromChildren<Mesh>()[0];
    mesh->vertices[1].texCoords.x = 15;
    mesh->vertices[2].texCoords.x = 15;
    mesh->vertices[2].texCoords.y = 15;
    mesh->vertices[3].texCoords.y = 15;

    mesh->material = planeMaterial;

    plane->addDataItem(planeMaterial);

    Hierarchy::addToHierarchy(plane);

    GameObject* extraGround = Hierarchy::createGameObjectInTree();
    extraGround->transform->scaleBy(100, 10, 100);
    extraGround->transform->translate(vec3(0, -10, 0));

    auto* groundCollider = new BoxCollider(100, 10, 100);
    groundCollider->setMass(0.);
    groundCollider->getRigidBody()->setFriction(1.);
    groundCollider->getRigidBody()->setRestitution(0.5);
    groundCollider->getRigidBody()->setRollingFriction(1);

    extraGround->components.add(groundCollider);

    // = datsun =
    datsun = loader->loadModel("models/datsun/source/datsun240k1.fbx");

    datsun->transform->scaleBy(0.0025);
    datsun->transform->translate(vec3(0, 0.8, 0));

    auto* tireMaterial = datsun->getDataItem<Material>(3);
    tireMaterial->diffuseTexture = *loader->loadTexture("models/datsun/textures/tire.png");
    tireMaterial->specularTexture = *loader->loadTexture("models/datsun/textures/tire.png");

    auto* licenseMaterial = datsun->getDataItem<Material>(6);
    licenseMaterial->diffuseTexture = *loader->loadTexture("models/datsun/textures/plate.png");

    auto* alloyMaterial = datsun->getDataItem<Material>(1);
    alloyMaterial->Kd = Vec3(189/255., 156/255., 70/255.);
    alloyMaterial->metalness = 1.0;
    alloyMaterial->roughness = 0.6;

    auto* chromeMaterial = datsun->getDataItem<Material>(5);
    chromeMaterial->metalness = 1.0;
    chromeMaterial->roughness = 0.8;

    auto* headlightMaterial = datsun->getDataItem<Material>(10);
    headlightMaterial->normalTexture = *loader->loadTexture("models/datsun/textures/headlight_normal.jpg");

    auto* stickersMaterial = datsun->getDataItem<Material>(11);
    stickersMaterial->diffuseTexture = *loader->loadTexture("models/datsun/textures/stickers.png");

    auto* datsunCollider = new BoxCollider(1, 1, 2.6);
    datsunCollider->setMass(8000);

    datsun->components.add(datsunCollider);

    Hierarchy::addToHierarchy(datsun);

    // = walls =
    auto* wall1 = createCube();
    auto* wall2 = createCube();
    auto* wall3 = createCube();
    auto* wall4 = createCube();

    wall1->transform->scaleBy(50, 3, 1);
    wall2->transform->scaleBy(50, 3, 1);
    wall3->transform->scaleBy(1, 3, 50);
    wall4->transform->scaleBy(1, 3, 50);

    wall1->transform->translate(vec3(0, 3, 50));
    wall2->transform->translate(vec3(0, 3, -50));
    wall3->transform->translate(vec3(50, 3, 0));
    wall4->transform->translate(vec3(-50, 3, 0));

    auto* wall1Collider = new BoxCollider(50, 3, 1);
    auto* wall2Collider = new BoxCollider(50, 3, 1);
    auto* wall3Collider = new BoxCollider(1, 3, 50);
    auto* wall4Collider = new BoxCollider(1, 3, 50);

    wall1Collider->setMass(10000);
    wall2Collider->setMass(10000);
    wall3Collider->setMass(10000);
    wall4Collider->setMass(10000);

    wall1Collider->getRigidBody()->setLinearFactor(btVector3(0, 0, 0));
    wall2Collider->getRigidBody()->setLinearFactor(btVector3(0, 0, 0));
    wall3Collider->getRigidBody()->setLinearFactor(btVector3(0, 0, 0));
    wall4Collider->getRigidBody()->setLinearFactor(btVector3(0, 0, 0));

    wall1Collider->getRigidBody()->setAngularFactor(btVector3(0, 0, 0));
    wall2Collider->getRigidBody()->setAngularFactor(btVector3(0, 0, 0));
    wall3Collider->getRigidBody()->setAngularFactor(btVector3(0, 0, 0));
    wall4Collider->getRigidBody()->setAngularFactor(btVector3(0, 0, 0));

    wall1Collider->getRigidBody()->setRestitution(0.5);
    wall2Collider->getRigidBody()->setRestitution(0.5);
    wall3Collider->getRigidBody()->setRestitution(0.5);
    wall4Collider->getRigidBody()->setRestitution(0.5);

    wall1Collider->getRigidBody()->setFriction(1.);
    wall2Collider->getRigidBody()->setFriction(1.);
    wall3Collider->getRigidBody()->setFriction(1.);
    wall4Collider->getRigidBody()->setFriction(1.);

    wall1Collider->getRigidBody()->setRollingFriction(1);
    wall2Collider->getRigidBody()->setRollingFriction(1);
    wall3Collider->getRigidBody()->setRollingFriction(1);
    wall4Collider->getRigidBody()->setRollingFriction(1);

    wall1->components.add(wall1Collider);
    wall2->components.add(wall2Collider);
    wall3->components.add(wall3Collider);
    wall4->components.add(wall4Collider);

    Hierarchy::addToHierarchy(wall1);
    Hierarchy::addToHierarchy(wall2);
    Hierarchy::addToHierarchy(wall3);
    Hierarchy::addToHierarchy(wall4);

    // player collider

    IEngine::speed = 0;

    player = Hierarchy::createGameObjectInTree();
    player->transform->translate(vec3(10, 10, 0));

    playerCollider = new BoxCollider(1, 2, 1);
    playerCollider->setMass(80.);
    playerCollider->getRigidBody()->setAngularFactor(btVector3(0, 1, 0));
    playerCollider->getRigidBody()->setFriction(0.);
    playerCollider->getRigidBody()->setActivationState(DISABLE_DEACTIVATION);

    player->components.add(playerCollider);

    // gun
    gun = createCube();

    gun->transform->scaleBy(0.15, 0.15, 0.5);

    Hierarchy::addToHierarchy(gun);

    // scene cubes
    vector<GameObject*> cubes;
    int N = 50;
    const float density = 500;

    for(int i=0; i < N; i++) {
        float size = getRandom(30, 60)/30.;

        auto* cubeMaterial = new Material();
        cubeMaterial->metalness = 0.8;
        cubeMaterial->roughness = 0.9;
        cubeMaterial->diffuseTexture = *loader->loadTexture("textures/aluminum/albedo.png");
        cubeMaterial->specularTexture = *loader->loadTexture("textures/aluminum/metallic.png");
        cubeMaterial->aoTexture = *loader->loadTexture("textures/aluminum/ao.png");
        cubeMaterial->roughnessTexture = *loader->loadTexture("textures/aluminum/roughness.png");
        cubeMaterial->normalTexture = *loader->loadTexture("textures/aluminum/normal.png");

        auto* cube = createCube();

        cube->components.getAllFromChildren<Mesh>()[0]->material = cubeMaterial;

        while(true) {
            bool intersect = false;

            float x = getRandom(10, 45) * (getRandom(0, 1) == 0 ? -1 : 1);
            float z = getRandom(10, 45) * (getRandom(0, 1) == 0 ? -1 : 1);

            for(auto* c : cubes) {
                float csize = c->transform->getScale().x;
                float cx = c->transform->getPosition().x;
                float cz = -c->transform->getPosition().z;

                if(doSquaresOverlap(x, z, size*2, cx, cz, csize*2)) {
                    intersect = true;
                    break;
                }
            }

            if(!intersect) {
                cube->transform->translate(vec3(x, size, z));
                break;
            }
        }

        cube->transform->scaleBy(size);

        auto* boxCollider = new BoxCollider(size, size, size);
        boxCollider->setMass(density*size*size*size);
        boxCollider->getRigidBody()->setFriction(1);
        boxCollider->getRigidBody()->setRollingFriction(1);
        boxCollider->getRigidBody()->setRestitution(0.2);
        cube->components.add(boxCollider);

        cubes.push_back(cube);
        Hierarchy::addToHierarchy(cube);
    }

    // = light source =
    sun = Hierarchy::createGameObjectInTree();

    Transform* lightTransform = sun->transform;
    lightTransform->translate(vec3(-8, 10, -10));
    lightTransform->rotate(vec3(radians(-60.0), radians(30.), 0));

    auto* directLight0 = new DirectLight(5);
    directLight0->ambient = vec3(0.2);

    sun->components.add(directLight0);

    // = flashlight =
    GameObject* flashlight = Hierarchy::createGameObjectInTree();
    PointLight* pointLight0 = PointLight::I1();
    flashlight->components.add(pointLight0);

    Hierarchy::setParent(flashlight, player);

    // = bullets
    for(int i=0; i < bulletCount; i++) {
        GameObject* bullet = createSphere();
        auto* bulletMaterial = new Material();
        bulletMaterial->normalTexture = *loader->loadTexture("textures/default_normal.png");
        bulletMaterial->diffuseTexture = *loader->loadTexture("textures/solid_white.png");
        bulletMaterial->specularTexture = *loader->loadTexture("textures/solid_white.png");
        bulletMaterial->roughnessTexture = *loader->loadTexture("textures/solid_white.png");
        bulletMaterial->aoTexture = *loader->loadTexture("textures/solid_white.png");

        bulletMaterial->Kd = Vec3(217/255., 116/255., 0);
        bulletMaterial->metalness = 1.0;
        bulletMaterial->roughness = 0.5;

        bullet->components.getAllFromChildren<Mesh>()[0]->material = bulletMaterial;
        bullet->addDataItem(bulletMaterial);

        bullet->transform->scaleBy(0.2);
        bullet->transform->setPosition(vec3(200, 3, 200));

        auto* bulletCollider = new SphereCollider(0.2);
        bulletCollider->setMass(400);
        bulletCollider->getRigidBody()->setLinearFactor(btVector3(0, 0, 0));
        bulletCollider->getRigidBody()->setFriction(1.);
        bulletCollider->getRigidBody()->setRollingFriction(1);
        bulletCollider->getRigidBody()->setDamping(0, 0.5);
        bulletCollider->getRigidBody()->setRestitution(1);

        auto* light = PointLight::I5();
        light->diffuse = vec3(217/255., 116/255., 0);

        bullet->components.add(light);
        bullet->components.add(bulletCollider);

        bullets.push_back(bullet);
        Hierarchy::addToHierarchy(bullet);
    }

    Scene::setup();
}

void FpsDemo::beforeRender() {
    IEngine::PhysicsEngine->dynamicsWorld->stepSimulation(IEngine::Input->getDeltaTime());

    // = player movement

    btVector3 playerPos = playerCollider->getRigidBody()->getWorldTransform().getOrigin();
    btVector3 rayEnd = playerPos - btVector3(0, 2, 0); // Check 1 meter below the player

    btCollisionWorld::ClosestRayResultCallback rayCallback(playerPos, rayEnd);

    IEngine::PhysicsEngine->dynamicsWorld->rayTest(playerPos, rayEnd, rayCallback);
    bool touchesGround = rayCallback.hasHit();

    if(input->axisHorizontal() == 0 && input->axisVertical() == 0) {
        const float verticalVelocity = playerCollider->getRigidBody()->getLinearVelocity().y();
        const auto velocity = btVector3(0, verticalVelocity, 0);

        playerCollider->getRigidBody()->setLinearVelocity(velocity);
    }
    else {
        float speed = IEngine::Input->isShiftPressed() ? 15.0f : 10.0f;
        IEngine::rotationSpeed = IEngine::Input->isShiftPressed() ? 0.15f : 0.3f;

        const vec3 proj = normalize(vec3(camera->transform->getDirectionVector().x, 0, camera->transform->getDirectionVector().z));
        const vec3 perp = normalize(cross(vec3(0, 1, 0), proj));

        const vec3 movement = normalize(proj * input->axisVertical() - perp * input->axisHorizontal()) * speed;
        const float verticalVelocity = playerCollider->getRigidBody()->getLinearVelocity().y();

        const auto velocity = btVector3(movement.x, verticalVelocity, movement.z);

        playerCollider->getRigidBody()->setLinearVelocity(velocity);
    }

    if(input->isSpacePressed() && touchesGround) {
        playerCollider->getRigidBody()->applyCentralForce(btVector3(0, 35000, 0));
    }

    camera->transform->setPosition(player->transform->getPosition());

    // = gun transform
    gun->transform->setPosition(camera->transform->getPosition());
    gun->transform->setRotation(camera->transform->getRotation());
    gun->transform->translate(vec3(0.9, -0.5, 1.5));

    if(input->isLeftMousePressed() && lastBulletTime > 0.25) {
        GameObject* bullet = bullets[bulletIndex];

        bullet->transform->setPosition(gun->transform->getPosition());

        auto* bulletCollider = bullet->components.get<SphereCollider>();

        bulletCollider->updateColliderTransform();
        bulletCollider->getRigidBody()->activate(true);
        bulletCollider->getRigidBody()->setLinearFactor(btVector3(1, 1, 1));
        bulletCollider->getRigidBody()->setRestitution(0.5);

        bulletCollider->getRigidBody()->clearForces();
        vec3 dir = gun->transform->getDirectionVector();
        btVector3 velocity = btVector3(dir.x, dir.y, dir.z) * 50;
        bulletCollider->getRigidBody()->setLinearVelocity(velocity);
        bulletCollider->getRigidBody()->setAngularVelocity(btVector3(0, 0, 0));

        lastBulletTime = 0;
        bulletIndex = (bulletIndex + 1) % bulletCount;
    }
    lastBulletTime += input->getDeltaTime();

    // datsun->transform->rotate(quat(vec3(0, radians(0.15f), 0)));

    Scene::beforeRender();
}
