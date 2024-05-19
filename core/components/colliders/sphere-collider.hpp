#pragma once

#include "collider.hpp"

class SphereCollider: public Collider {
private:
    float radius;

public:
    explicit SphereCollider(float radius);

    void setRadius(float radius);

protected:
    btCollisionShape* createShape() override;
};
