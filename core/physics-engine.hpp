#pragma once

#include "btBulletDynamicsCommon.h"

class PhysicsEngine {
private:

    btDefaultCollisionConfiguration* collisionConfiguration;

    btCollisionDispatcher* dispatcher;

    btBroadphaseInterface* overlappingPairCache;

    btSequentialImpulseConstraintSolver* solver;

    btVector3 gravity = btVector3(0, -15, 0);

    btAlignedObjectArray<btCollisionShape*> collisionShapes;

public:
    // TODO: make private
    btDiscreteDynamicsWorld* dynamicsWorld;

    PhysicsEngine();

    ~PhysicsEngine();

public:
    void addRigidBody(btRigidBody* rigidBody) const;

    void removeRigidBody(btRigidBody* rigidBody) const;

private:
};