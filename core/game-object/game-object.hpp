#pragma once

class GameObject {
private:
    static inline int lastID = 0;
    
    static int generateId() {
        return lastID++;
    }
    
public:
    int ID;
    
    GameObject();
};
