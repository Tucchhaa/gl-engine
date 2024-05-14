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

GameObject* Hierarchy::createGameObjectInTree() {
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
    setParent(gameObject, root);

    gameObject->traverseChildren(addToGameObjects);
}

GameObject* Hierarchy::getParent(const GameObject* gameObject) {
    return gameObject->parent;
}

void Hierarchy::setParent(GameObject* child, GameObject* parent) {
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

void Hierarchy::setParent(const vector<GameObject*>& children, GameObject* parent) {
    for(auto* child: children) {
        setParent(child, parent);
    }
}

void Hierarchy::updateTransformTree() {
    root->traverseChildren(updateTransform);
}

void Hierarchy::updateTransformTree(GameObject* gameObject) {
    gameObject->traverseChildren(updateTransform);
}

void Hierarchy::updateTransform(GameObject* gameObject) {
    if(gameObject == root || gameObject->parent == nullptr)
        return;

    gameObject->transform->updateAbsoluteValues(gameObject->parent->transform);
}

void Hierarchy::addToGameObjects(GameObject* gameObject) {
    gameObjects[gameObject->ID] = gameObject;
}
