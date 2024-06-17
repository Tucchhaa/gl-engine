#pragma once

#include "btBulletDynamicsCommon.h"

#include "../component.hpp"

class Collider: public ObjectComponent {
private:
    float mass = 1.;

    btCollisionShape* shape = nullptr;

protected:
    btRigidBody* rigidBody = nullptr;

public:
    Collider();

    ~Collider() override;

    void setMass(float mass);

public:
    void onGameObjectSet() override;

    void beforeRender() override;

    // TODO: remove this method
    btRigidBody* getRigidBody() const;

    void updateColliderTransform() const;

private:
    btTransform getColliderTransform() const;

    /**
     * Updates transform of game object from collider transform
     */
    void updateTransformFromCollider() const;

    bool isDynamic() const;

protected:
    btRigidBody* createRigidBody();

    virtual btCollisionShape* createShape() = 0;
};


