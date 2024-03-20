#pragma once

class ObjectComponent {
private:
    static inline int componentId = 0;
    
    static int generateComponentId() {
        return componentId++;
    }

public:
    int GameObjectID;
    
    int ComponentID;
    
    ObjectComponent();
    
    virtual void func() {}
};
