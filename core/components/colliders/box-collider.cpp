#include "box-collider.hpp"

BoxCollider::BoxCollider(float width, float height, float depth):
    width(width), height(height), depth(depth) {
    rigidBody = createRigidBody();
}

btCollisionShape* BoxCollider::createShape() {
    return new btBoxShape(btVector3(width, height, depth));
}
