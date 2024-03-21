#pragma once

#include <vector>
#include <map>

#include "./game-object.hpp"
#include "./components/component.hpp"
#include "./components/transform.hpp"
#include "./components/mesh.hpp"
#include "./components/lights/direct-light.hpp"
#include "./components/lights/point-light.hpp"

using namespace std;

class Hierarchy {
private:
    /**
     * Key - gameObjectID, value - gameObject's components
     */
    static map<int, vector<ObjectComponent*>> components;
    
    /**
     * Key - gameObjectID, value - gameObject
     */
    static map<int, GameObject*> gameObjects;
    
    template<typename T>
    static vector<T*> _getComponents(int objectId, bool all, bool required = false);
    
public:
    static vector<Mesh*> getMeshes(int objectId);
    static vector<DirectLight*> getDirectLights(int objectId);
    static vector<PointLight*> getPointLights(int objectId);
    
    // ===
    
    // TODO: limit T type to ObjectComponent
    template<typename T>
    static T* getComponent(int objectId);
    
    template<typename T>
    static T* getComponent(const GameObject* gameObject);
    
    template<typename T>
    static T* getRequiredComponent(const GameObject* gameObject);
    
    // ===
    
    template<typename T>
    static vector<T*> getComponents(int objectId);
    
    // ===
    
    static Transform* getTransform(int objectId);
    
    static Transform* getTransform(const ObjectComponent* component);
    
    static Transform* getTransform(const GameObject* gameObject);
    
    // ===
    
    static GameObject* getGameObject(int objectId);
    
    static GameObject* getGameObject(const ObjectComponent* component);
    
    static map<int, GameObject*>* getGameObjects();
    
    // ===
    
    static void addGameObject(GameObject* gameObject);
    
    static void addComponent(int objectId, ObjectComponent* component);
    
    static void addComponent(GameObject* gameObject, ObjectComponent* component);
};

