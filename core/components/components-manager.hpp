#pragma once

#include <vector>

#include "component.hpp"


using namespace  std;

class ComponentsManager {
private:
    GameObject* gameObject;

    vector<ObjectComponent*> components;

public:
    explicit ComponentsManager(GameObject* gameObject);

    ~ComponentsManager();

public:
    template<typename T>
    T* get();

    template<typename T>
    T* getRequired();

    template<typename T>
    vector<T*> getAll();

    template<typename T>
    vector<T*> getAllFromChildren();

    void add(ObjectComponent* component);

private:
    /**
     * returns components of objectId
     */
    template<typename T>
    vector<T*> _get(bool all, bool required = false);
};


