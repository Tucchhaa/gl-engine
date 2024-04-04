#include "hierarchy.hpp"

#include "./game-object/components/mesh.hpp"
#include "./game-object/components/terrain.hpp"
#include "./game-object/components/cubic-patch.hpp"

#include "./game-object/components/camera.hpp"

#include "./game-object/components/lights/direct-light.hpp"
#include "./game-object/components/lights/point-light.hpp"
#include "./game-object/components/lights/spot-light.hpp"

map<int, vector<ObjectComponent*>> Hierarchy::components;

map<int, GameObject*> Hierarchy::gameObjects;

GameObject* Hierarchy::root;

void Hierarchy::initialize() {
    root = createRoot();
}

// ===
// Components Getters
// ===

template<typename T>
vector<T*> Hierarchy::Components<T>::_get(int objectId, bool all, bool required) {
    vector<T*> result;
    
    for(ObjectComponent* component : Hierarchy::components[objectId]) {
        bool isSameType = typeid(*component) == typeid(T);
        
        if(isSameType) {
            result.push_back(static_cast<T*>(component));
            
            if(!all)
                return result;
        }
    }
    
    if(required && result.size() == 0) {
        throw std::runtime_error("No ObjectComponent found with the provided type");
    }
    
    return result;
}

template<typename T>
T* getFrontOrNull(vector<T*> array) {
    return array.size() > 0 ? array[0] : nullptr;
}

template<typename T>
Hierarchy::Components<T>::Components() = default;

template<typename T>
T* Hierarchy::Components<T>::get(int objectId) {
    return getFrontOrNull(_get(objectId, false));
}

template<typename T>
T* Hierarchy::Components<T>::get(const GameObject* gameObject) {
    return get(gameObject->ID);
}

template<typename T>
T* Hierarchy::Components<T>::getRequired(int objectId) {
    return _get(objectId, false, true)[0];
}

template<typename T>
T* Hierarchy::Components<T>::getRequired(const GameObject* gameObject) {
    return _get(gameObject->ID, false, true)[0];
}

template<typename T>
vector<T*> Hierarchy::Components<T>::getAll(int objectId) {
    return _get(objectId, true);
}

// ===
//
// ===
Transform* Hierarchy::getTransform(int objectId) {
    return Components<Transform>::getRequired(objectId);
}

Transform* Hierarchy::getTransform(const GameObject* gameObject) {
    return getTransform(gameObject->ID);
}

Transform* Hierarchy::getTransform(const ObjectComponent* component) {
    return getTransform(component->GameObjectID);
}

// ===

GameObject* Hierarchy::getGameObject(int objectId) {
    return Hierarchy::gameObjects[objectId];
}

GameObject* Hierarchy::getGameObject(const ObjectComponent* component) {
    return getGameObject(component->GameObjectID);
}

map<int, GameObject*>* Hierarchy::getGameObjects() {
    return &gameObjects;
}

// ===

GameObject* Hierarchy::createGameObject() {
    auto* transform = new Transform();
    auto* gameObject = new GameObject(transform);

    Hierarchy::gameObjects[gameObject->ID] = gameObject;

    addComponent(gameObject, transform);

    setParent(root, gameObject);

    return gameObject;
}

GameObject* Hierarchy::createRoot() {
    auto* transform = new Transform();
    auto* gameObject = new GameObject(transform);
    gameObject->ID = 0;

    Hierarchy::gameObjects[gameObject->ID] = gameObject;

    addComponent(gameObject, transform);

    return gameObject;
}

void Hierarchy::addComponent(int objectId, ObjectComponent* component) {
    component->GameObjectID = objectId;
    
    Hierarchy::components[objectId].push_back(component);
}

void Hierarchy::addComponent(GameObject* gameObject, ObjectComponent* component) {
    addComponent(gameObject->ID, component);
}

// === Hierarchy tree operations ===

GameObject* Hierarchy::getParent(GameObject* gameObject) {
    return getGameObject(gameObject->parentID);
}

GameObject* Hierarchy::getParent(int objectId) {
    return getGameObject(getGameObject(objectId)->parentID);
}

void Hierarchy::setParent(GameObject* parent, GameObject* child) {
    getGameObject(child->parentID)->children.erase(child->ID);

    parent->children.insert(child->ID);
    child->parentID = parent->ID;
}

void Hierarchy::updateTransformTree() {
    updateTransformTree(root->transform);
}

void Hierarchy::updateTransformTree(Transform* transform) {
    Transform* parentTransform;

    if(transform->GameObjectID != root->ID) {
        parentTransform = getParent(transform->GameObjectID)->transform;
        transform->updateAbsoluteValues(parentTransform);
    }

    queue<Transform*> q;
    q.push(transform);

    while(!q.empty()) {
        parentTransform = q.front();

        q.pop();

        for(auto childId: getGameObject(parentTransform->GameObjectID)->children) {
            Transform* childTransform = getTransform(childId);

            childTransform->updateAbsoluteValues(parentTransform);

            q.push(childTransform);
        }
    }
}

// ===

template class Hierarchy::Components<Transform>;

template class Hierarchy::Components<Camera>;

template class Hierarchy::Components<Mesh>;
template class Hierarchy::Components<Terrain>;
template class Hierarchy::Components<CubicPatch>;

template class Hierarchy::Components<DirectLight>;
template class Hierarchy::Components<PointLight>;
template class Hierarchy::Components<SpotLight>;
