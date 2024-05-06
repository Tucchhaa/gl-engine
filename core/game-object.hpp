#pragma once

#include <set>

#include "components/transform.hpp"
#include "structures/material.hpp"

using namespace std;

class GameObject {
private:
    static inline int lastID = 1;
    
    static int generateId() {
        return lastID++;
    }
    
public:
    int ID;

    GameObject* parent = nullptr;

    set<GameObject*> children;

    Transform* transform = nullptr;

    /**
     * Contains pointers to data, that children use. It has Materials.
     */
    vector<void*> data;

public:
    explicit GameObject(Transform* transform);

public:
    template<typename T>
    T* getData(int index = 0) const;
};
