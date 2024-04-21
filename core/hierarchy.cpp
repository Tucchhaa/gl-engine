#include "hierarchy.hpp"

#include <queue>

#include "./components/mesh.hpp"
#include "./components/terrain.hpp"
#include "./components/cubic-patch.hpp"

#include "./components/camera.hpp"

#include "./components/lights/direct-light.hpp"
#include "./components/lights/point-light.hpp"
#include "./components/lights/spot-light.hpp"

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
vector<T*> Hierarchy::Components<T>::_get(const int objectId, const bool all, bool required) {
    vector<T*> result;
    
    for(ObjectComponent* component : components[objectId]) {
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
T* Hierarchy::Components<T>::get(const int objectId) {
    return getFrontOrNull(_get(objectId, false));
}

template<typename T>
T* Hierarchy::Components<T>::get(const GameObject* gameObject) {
    return get(gameObject->ID);
}

template<typename T>
T* Hierarchy::Components<T>::getRequired(const int objectId) {
    return _get(objectId, false, true)[0];
}

template<typename T>
T* Hierarchy::Components<T>::getRequired(const GameObject* gameObject) {
    return _get(gameObject->ID, false, true)[0];
}

template<typename T>
vector<T*> Hierarchy::Components<T>::getAll(const int objectId) {
    return _get(objectId, true);
}

// ===
// Transform Getters
// ===
Transform* Hierarchy::getTransform(const int objectId) {
    return Components<Transform>::getRequired(objectId);
}

Transform* Hierarchy::getTransform(const GameObject* gameObject) {
    return gameObject->transform;
}

Transform* Hierarchy::getTransform(const ObjectComponent* component) {
    return component->transform;
}

// ===

GameObject* Hierarchy::getGameObject(int objectId) {
    return gameObjects[objectId];
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

    gameObjects[gameObject->ID] = gameObject;

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

void Hierarchy::addComponent(const int objectId, ObjectComponent* component) {
    addComponent(getGameObject(objectId), component);
}

void Hierarchy::addComponent(GameObject* gameObject, ObjectComponent* component) {
    component->GameObjectID = gameObject->ID;

    component->gameObject = gameObject;
    component->transform = gameObject->transform;

    components[gameObject->ID].push_back(component);
}

// === Hierarchy tree operations ===

GameObject* Hierarchy::getParent(GameObject* gameObject) {
    return gameObject->parent;
}

GameObject* Hierarchy::getParent(int objectId) {
    return getGameObject(objectId)->parent;
}

void Hierarchy::setParent(GameObject* parent, GameObject* child) {
    // remove from old parent
    if(child->parent != nullptr) {
        set<GameObject*> oldSiblings = child->parent->children;

        auto it = oldSiblings.find(child);
        if(it != oldSiblings.end()) {
            oldSiblings.erase(child);
        }
    }

    parent->children.insert(child);
    child->parent = parent;
}

void Hierarchy::updateTransformTree() {
    updateTransformTree(root);
}

void Hierarchy::updateTransformTree(GameObject* gameObject) {
    // update transform of itself
    if(gameObject != root) {
        gameObject->transform->updateAbsoluteValues(gameObject->parent->transform);
    }

    queue<GameObject*> q;
    q.push(gameObject);

    while(!q.empty()) {
        const GameObject* parent = q.front();

        q.pop();

        for(const auto child: parent->children) {
            child->transform->updateAbsoluteValues(parent->transform);

            q.push(child);
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
