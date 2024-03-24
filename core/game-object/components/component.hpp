#pragma once

class ObjectComponent {
private:
    static inline int componentId = 1;
    
    static int generateComponentId() {
        return componentId++;
    }

public:
    int GameObjectID = 0;
    
    int ComponentID;
    
    ObjectComponent();

    // === Need this to make ObjectComponent polymorphic, so the dynamic_cast will work ===
    virtual void func() {}
};
