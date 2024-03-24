#pragma once

#include <set>

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

public:
    GameObject();
};
