#include "game-object.hpp"

GameObject::GameObject(Transform* transform): transform(transform) {
    ID = generateId();

    parentID = 0;
}
