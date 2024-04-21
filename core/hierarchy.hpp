#pragma once

#include <vector>
#include <map>

#include "./game-object/game-object.hpp"

#include "./game-object/components/component.hpp"
#include "./game-object/components/transform.hpp"

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

    static GameObject* root;

public:
    static void initialize();

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

        static T* getRequired(int objectId);
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
    
    static GameObject* createGameObject();

    static GameObject* createRoot();
    
    static void addComponent(int objectId, ObjectComponent* component);
    static void addComponent(GameObject* gameObject, ObjectComponent* component);

    // === Hierarchy tree operations ===

    static GameObject* getParent(GameObject* gameObject);
    static GameObject* getParent(int objectId);

    static void setParent(GameObject* parent, GameObject* child);

    static void updateTransformTree();
    static void updateTransformTree(Transform* transform);
};
