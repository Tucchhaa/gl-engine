#pragma once

#include <vector>
#include <map>

#include "./game-object.hpp"
#include "./components/component.hpp"
#include "./components/transform.hpp"

using namespace std;

class Hierarchy {
private:
    static map<int, vector<ObjectComponent*>> components;
    
    static map<int, GameObject*> gameObjects;
    
public:
    // TODO: limit T type to ObjectComponent
    template<typename T>
    static T getComponent(int objectId);
    
    template<typename T>
    static T getComponent(const GameObject* gameObject);
    
    static Transform* getTransform(int objectId);
    
    static Transform* getTransform(const GameObject* gameObject);
    
    static GameObject* getGameObject(int objectId);
    
    static GameObject* getGameObject(const ObjectComponent* component);
    
    static void addGameObject(GameObject* gameObject);
    
    static void addComponent(int objectId, ObjectComponent* component);
    
    static void addComponent(GameObject* gameObject, ObjectComponent* component);
};

