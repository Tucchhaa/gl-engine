#include "hierarchy.hpp"

map<int, vector<ObjectComponent*>> Hierarchy::components;

map<int, GameObject*> Hierarchy::gameObjects;

// ===

template<typename T>
vector<T*> Hierarchy::Components<T>::_get(int objectId, bool all, bool required) {
    vector<T*> components;
    
    for(ObjectComponent* component : Hierarchy::components[objectId]) {
        T* result = dynamic_cast<T*>(component);
        
        if(result != nullptr) {
            components.push_back(result);
            
            if(!all)
                return components;
        }
    }
    
    if(required && components.size() == 0) {
        throw std::runtime_error("No ObjectComponent found with the provided type");
    }
    
    return components;
}

template<typename T>
T* getFrontOrNull(vector<T*> array) {
    return array.size() > 0 ? array[0] : nullptr;
}

template<typename T>
T* Hierarchy::Components<T>::get(int objectId) {
    return getFrontOrNull(_get(objectId, false));
}

template<typename T>
T* Hierarchy::Components<T>::get(const GameObject* gameObject) {
    return get(gameObject->ID);
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

Transform* Hierarchy::getTransform(int objectId) {
    return Components<Transform>::get(objectId);
}

Transform* Hierarchy::getTransform(const GameObject* gameObject) {
    return getTransform(gameObject->ID);
}

Transform* Hierarchy::getTransform(const ObjectComponent* component) {
    return getTransform(getGameObject(component->GameObjectID));
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

template class Hierarchy::Components<Mesh>;
template class Hierarchy::Components<DirectLight>;
template class Hierarchy::Components<PointLight>;
