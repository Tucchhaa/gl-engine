#include "hierarchy.hpp"

map<int, vector<ObjectComponent*>> Hierarchy::components;

map<int, GameObject*> Hierarchy::gameObjects;


template<typename T>
vector<T*> Hierarchy::_getComponents(int objectId, bool all, bool required) {
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
T* Hierarchy::getComponent(int objectId) {
    return getFrontOrNull(_getComponents<T>(objectId, false));
}

template<typename T>
T* Hierarchy::getComponent(const GameObject* gameObject) {
    return getComponent<T>(gameObject->ID);
}

template<typename T>
T* Hierarchy::getRequiredComponent(const GameObject* gameObject) {
    return _getComponents<T>(gameObject->ID, false, true)[0];
}

template<typename T>
vector<T*> Hierarchy::getComponents(int objectId) {
    return _getComponents<T>(objectId, true);
}


Transform* Hierarchy::getTransform(int objectId) {
    return getComponent<Transform>(objectId);
}
vector<Mesh*> Hierarchy::getMeshes(int objectId) {
    return getComponents<Mesh>(objectId);
}
vector<DirectLight*> Hierarchy::getDirectLights(int objectId) {
    return getComponents<DirectLight>(objectId);
}
vector<PointLight*> Hierarchy::getPointLights(int objectId) {
    return getComponents<PointLight>(objectId);
}

Transform* Hierarchy::getTransform(const GameObject* gameObject) {
    return getTransform(gameObject->ID);
}

Transform* Hierarchy::getTransform(const ObjectComponent* component) {
    return getTransform(getGameObject(component->GameObjectID));
}

GameObject* Hierarchy::getGameObject(int objectId) {
    return Hierarchy::gameObjects[objectId];
}

GameObject* Hierarchy::getGameObject(const ObjectComponent* component) {
    return getGameObject(component->GameObjectID);
}

map<int, GameObject*>* Hierarchy::getGameObjects() {
    return &gameObjects;
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
