#include "game-object.hpp"

#include <queue>
#include "structures/material.hpp"

GameObject::GameObject(Transform* transform): transform(transform), components(this) {
    ID = generateId();
    components.add(transform);
}

/**
 * Invokes callback for game object and its children
 */
void GameObject::traverseChildren(const function<void(GameObject*)> &callback) {
    queue<GameObject*> q;
    q.push(this);

    while(!q.empty()) {
        GameObject* current = q.front();
        q.pop();

        callback(current);

        for(auto* child: current->children)
            q.push(child);
    }
}

GameObject::GameObject(): transform(new Transform()), components(this) {
    ID = generateId();
    components.add(transform);
}

template<typename T>
vector<T*>* GameObject::getDataList() {
    const auto T_type = type_index(typeid(T));

    return reinterpret_cast<vector<T*>*>(&dataMap[T_type]);
}

template<typename T>
T* GameObject::getDataItem(int index) {
    return (*getDataList<T>())[index];
}

template<typename T>
void GameObject::addDataItem(T* item) {
    vector<T*>* list = getDataList<T>();

    list->push_back(item);
}

template vector<Material*>* GameObject::getDataList();
template vector<Vec3*>* GameObject::getDataList();

template Material* GameObject::getDataItem(int index);
template Vec3* GameObject::getDataItem(int index);

template void GameObject::addDataItem(Material* item);
template void GameObject::addDataItem(Vec3* item);
