#pragma once

#include <vector>
#include <map>

#include "./game-object.hpp"

#include "./components/component.hpp"

using namespace std;

class Hierarchy {
private:
    /**
     * Game objects in the hierarchy tree. Key - gameObjectID, value - gameObject
     */
    static map<int, GameObject*> gameObjects;

    /**
     * Root of the hierarchy tree
     */
    static GameObject* root;

public:
    static void initialize();

public:
    /**
     * Get game objects in the hierarchy tree
     */
    static map<int, GameObject*>* getGameObjects();
    
    // ===

    /**
     * Creates new game object and adds it to the hierarchy tree
     * @return created game object
     */
    static GameObject* createGameObjectInTree();

    static GameObject* createRoot();

    // === Hierarchy tree operations ===

    /**
    * Adds game object and its children to the hierarchy tree.
    */
    static void addToHierarchy(GameObject* gameObject);

    static GameObject* getParent(const GameObject* gameObject);

    static void setParent(GameObject* child, GameObject* parent);

    static void updateTransformTree();
    static void updateTransformTree(GameObject* transform);
};
