#include "hierarchy.hpp"

#include <queue>

map<int, GameObject*> Hierarchy::gameObjects;

GameObject* Hierarchy::root;

void Hierarchy::initialize() {
    root = createRoot();
}


// ===
map<int, GameObject*>* Hierarchy::getGameObjects() {
    return &gameObjects;
}

GameObject* Hierarchy::addGameObject() {
    auto* gameObject = new GameObject();

    addToHierarchy(gameObject);

    return gameObject;
}

GameObject* Hierarchy::createRoot() {
    auto* root = new GameObject();
    root->ID = 0;

    gameObjects[root->ID] = root;

    return root;
}

// === Hierarchy tree operations ===

void Hierarchy::addToHierarchy(GameObject* gameObject) {
    gameObjects[gameObject->ID] = gameObject;

    setParent(root, gameObject);
}

GameObject* Hierarchy::getParent(const GameObject* gameObject) {
    return gameObject->parent;
}

void Hierarchy::setParent(GameObject* parent, GameObject* child) {
    // remove from old parent
    if(child->parent != nullptr) {
        set<GameObject*>* oldSiblings = &child->parent->children;

        const auto it = oldSiblings->find(child);

        if(it != oldSiblings->end()) {
            oldSiblings->erase(child);
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
