#include "game-object.hpp"

#include "structures/material.hpp"

GameObject::GameObject(Transform* transform): transform(transform) {
    ID = generateId();
}

template<typename T>
T* GameObject::getData(const int index) const {
    int currentIndex = 0;

    for(void* item: data) {
        T* castedItem = static_cast<T*>(item);
        const bool isSameType = typeid(*castedItem) == typeid(T);

        if(isSameType) {
            if(index == currentIndex) {
                return castedItem;
            }

            currentIndex++;
        }
    }

    return nullptr;
}

template Material* GameObject::getData<Material>(int) const;
