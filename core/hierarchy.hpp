#pragma once

#include <vector>
#include <map>

#include "./game-object/game-object.hpp"

#include "./game-object/components/component.hpp"
#include "./game-object/components/transform.hpp"
#include "./game-object/components/mesh.hpp"
#include "./game-object/components/lights/direct-light.hpp"
#include "./game-object/components/lights/point-light.hpp"

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
    
    
public:
    // TODO: limit T type to ObjectComponent
    template<typename T>
    class Components {
    private:
        /**
         * returns components of objectId
         */
        static vector<T*> _get(int objectId, bool all, bool required = false);
        
    public:
        Components();
        
        static T* get(int objectId);
        
        static T* get(const GameObject* gameObject);
        
        static T* getRequired(const GameObject* gameObject);
        
        static vector<T*> getAll(int objectId);
    };
public:
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
