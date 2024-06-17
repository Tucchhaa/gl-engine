#pragma once
#include "../core/scene.hpp"
#include "../core/components/colliders/box-collider.hpp"

class FpsDemo : public Scene {
private:
    GameObject* datsun;

    GameObject* gun;

    GameObject* player;

    BoxCollider* playerCollider;

    GameObject* sun;

    vector<GameObject*> bullets;

    int bulletIndex = 0;

    int bulletCount = 30;

    float lastBulletTime = 0;

public:
    FpsDemo() = default;

    void setup() override;

    void beforeRender() override;
};
