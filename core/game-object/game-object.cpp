#include "game-object.hpp"

GameObject::GameObject() {
    ID = generateId();

    parentID = 0;
}
