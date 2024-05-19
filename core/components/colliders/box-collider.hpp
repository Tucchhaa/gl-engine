#pragma once

#include "collider.hpp"

class BoxCollider: public Collider {
private:
    float width;

    float height;

    float depth;

public:
    BoxCollider(float width, float height, float depth);

protected:
    btCollisionShape* createShape() override;
};