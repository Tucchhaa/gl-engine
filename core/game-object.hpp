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

    int parentID;

    set<int> children;

    Transform* transform;

public:
    explicit GameObject(Transform* transform);
};
