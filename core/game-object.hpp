#pragma once

#include <set>
#include <typeindex>

#include "components/components-manager.hpp"
#include "components/transform.hpp"

using namespace std;

class GameObject {
    friend class Hierarchy;

private:
    static inline int lastID = 1;
    
    static int generateId() {
        return lastID++;
    }

    /**
     * Can pointers to data, that object and children use.
     * It has Materials.
     */
    unordered_map<type_index, vector<void*>> dataMap;

public:
    int ID;

    GameObject* parent = nullptr;

    set<GameObject*> children;

    Transform* transform;

    ComponentsManager components;

public:
    GameObject();
    explicit GameObject(Transform* transform);

public:
    void traverseChildren(const function<void(GameObject*)> &callback);

    template<typename T>
    vector<T*>* getDataList();

    template<typename T>
    T* getDataItem(int index = 0);

    template<typename T>
    void addDataItem(T* item);
};
