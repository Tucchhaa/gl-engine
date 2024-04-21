#pragma once

#include <set>

#include "./components/transform.hpp"

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

public:
    explicit GameObject(Transform* transform);
};
