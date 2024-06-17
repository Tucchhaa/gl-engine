#include "collider.hpp"

#include "../transform.hpp"
#include "../../hierarchy.hpp"
#include "../../../apis/base/iengine.hpp"

Collider::Collider() = default;

Collider::~Collider() {
    if(rigidBody->getMotionState())
        delete rigidBody->getMotionState();

    IEngine::PhysicsEngine->removeRigidBody(rigidBody);

    delete rigidBody;
    delete shape;
}

void Collider::setMass(float mass) {
    this->mass = mass;

    IEngine::PhysicsEngine->removeRigidBody(rigidBody);

    btVector3 inertia;
    rigidBody->getCollisionShape()->calculateLocalInertia( mass, inertia );
    rigidBody->setMassProps(btScalar(mass), inertia);

    IEngine::PhysicsEngine->addRigidBody(rigidBody);
}

void Collider::onGameObjectSet() {
    ObjectComponent::onGameObjectSet();

    updateColliderTransform();
}

void Collider::beforeRender() {
    updateTransformFromCollider();

    ObjectComponent::beforeRender();
}

btRigidBody* Collider::getRigidBody() const {
    return rigidBody;
}

void Collider::updateColliderTransform() const {
    const btTransform colliderTransform = getColliderTransform();
    rigidBody->setWorldTransform(colliderTransform);
}

btTransform Collider::getColliderTransform() const {
    btTransform colliderTransform;

    const vec3 position = transform->getPosition();
    const quat rotation = transform->getRotation();

    const btVector3 btPosition(position.x, position.y, position.z);
    const btQuaternion btRotation(rotation.x, rotation.y, rotation.z, rotation.w);

    colliderTransform.setOrigin(btPosition);
    colliderTransform.setRotation(btRotation);

    return colliderTransform;
}

btRigidBody* Collider::createRigidBody() {
    shape = createShape();

    if(shape == nullptr)
        throw std::runtime_error("Shape is nullptr");

    btVector3 localInertia(0, 0, 0);
    if (isDynamic())
        shape->calculateLocalInertia(mass, localInertia);

    auto* motionState = new btDefaultMotionState(btTransform::getIdentity());
    const btRigidBody::btRigidBodyConstructionInfo info(mass, motionState, shape, localInertia);
    auto* rigidBody = new btRigidBody(info);

    IEngine::PhysicsEngine->addRigidBody(rigidBody);

    return rigidBody;
}

void Collider::updateTransformFromCollider() const {
    btTransform colliderTransform;

    if (rigidBody && rigidBody->getMotionState())
        rigidBody->getMotionState()->getWorldTransform(colliderTransform);
    else
        colliderTransform = rigidBody->getWorldTransform();

    const btVector3 position = colliderTransform.getOrigin();
    const btQuaternion rotation = colliderTransform.getRotation();

    transform->setPosition(position.x(), position.y(), position.z());
    transform->setRotation(rotation.w(), rotation.x(), rotation.y(), rotation.z());
}

bool Collider::isDynamic() const {
    return mass != 0.f;
}
