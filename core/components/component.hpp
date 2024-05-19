#pragma once
#include "../igame-events-listener.hpp"

class GameObject;
class Transform;

class ObjectComponent: public IGameEventsListener {
private:
    static inline int componentId = 1;
    
    static int generateComponentId() {
        return componentId++;
    }

public:
    int GameObjectID = 0;
    
    int ComponentID;

    GameObject* gameObject = nullptr;
    Transform* transform = nullptr;

    ObjectComponent();

public:
    virtual void onGameObjectSet();

// ===
// implement IGameEventsListener
// ===
public:
    void setupScene() override;

    void beforeRender() override;

    void afterRender() override;
};
