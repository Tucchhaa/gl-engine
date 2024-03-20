#include "hierarchy.hpp"

map<int, vector<ObjectComponent*>> Hierarchy::components;

map<int, GameObject*> Hierarchy::gameObjects;

template<typename T>
T Hierarchy::getComponent(int objectId) {
    for(ObjectComponent* component : Hierarchy::components[objectId]) {
        T result = dynamic_cast<T>(component);
        
        if(result != nullptr) {
            return result;
        }
    }
    
    throw std::runtime_error("No ObjectComponent found with the provided type");
}

template<typename T>
T Hierarchy::getComponent(const GameObject* gameObject) {
    return getComponent<T>(gameObject->ID);
}

Transform* Hierarchy::getTransform(int objectId) {
    return getComponent<Transform*>(objectId);
}

Transform* Hierarchy::getTransform(const GameObject* gameObject) {
    return getTransform(gameObject->ID);
}

GameObject* Hierarchy::getGameObject(int objectId) {
    return Hierarchy::gameObjects[objectId];
}

GameObject* Hierarchy::getGameObject(const ObjectComponent* component) {
    return getGameObject(component->GameObjectID);
}

void Hierarchy::addGameObject(GameObject* gameObject) {
    Hierarchy::gameObjects[gameObject->ID] = gameObject;
}

void Hierarchy::addComponent(int objectId, ObjectComponent* component) {
    component->GameObjectID = objectId;
    
    Hierarchy::components[objectId].push_back(component);
}

void Hierarchy::addComponent(GameObject* gameObject, ObjectComponent* component) {
    addComponent(gameObject->ID, component);
}
