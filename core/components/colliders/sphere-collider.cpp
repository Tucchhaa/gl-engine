#include "sphere-collider.hpp"

SphereCollider::SphereCollider(float radius): Collider(), radius(radius) {
    rigidBody = createRigidBody();
}

void SphereCollider::setRadius(float radius) {
    this->radius = radius;
}

btCollisionShape* SphereCollider::createShape() {
    return new btSphereShape(radius);
}
