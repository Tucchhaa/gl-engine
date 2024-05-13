#include "components-manager.hpp"

#include "transform.hpp"
#include "camera.hpp"
#include "terrain.hpp"
#include "cubic-patch.hpp"
#include "mesh.hpp"
#include "../game-object.hpp"
#include "lights/direct-light.hpp"
#include "lights/point-light.hpp"
#include "lights/spot-light.hpp"

ComponentsManager::ComponentsManager(GameObject* gameObject): gameObject(gameObject) {}

ComponentsManager::~ComponentsManager() {
    for(const ObjectComponent* component : components) {
        delete component;
    }
}

template<typename T>
vector<T*> ComponentsManager::_get(const bool all, bool required) {
    vector<T*> result;

    for(ObjectComponent* component : components) {
        const bool isSameType = typeid(*component) == typeid(T);

        if(isSameType) {
            result.push_back(static_cast<T*>(component));

            if(!all)
                return result;
        }
    }

    if(required && result.size() == 0) {
        throw std::runtime_error("No ObjectComponent found with the provided type");
    }

    return result;
}

template<typename T>
T* getFrontOrNull(vector<T*> array) {
    return array.size() > 0 ? array[0] : nullptr;
}

template<typename T>
T* ComponentsManager::get() {
    return getFrontOrNull(_get<T>(false));
}

template<typename T>
T* ComponentsManager::getRequired() {
    return _get<T>(false, true)[0];
}

template<typename T>
vector<T*> ComponentsManager::getAll() {
    return _get<T>(true);
}

void ComponentsManager::add(ObjectComponent* component) {
    components.push_back(component);

    component->gameObject = gameObject;
    component->GameObjectID = gameObject->ID;
    component->transform = gameObject->transform;
}

#define INSTANTIATE_COMPONENTS_MANAGER_FUNCTIONS(T) \
template T* ComponentsManager::get<T>(); \
template T* ComponentsManager::getRequired<T>(); \
template vector<T*> ComponentsManager::getAll<T>();

INSTANTIATE_COMPONENTS_MANAGER_FUNCTIONS(Transform);
INSTANTIATE_COMPONENTS_MANAGER_FUNCTIONS(Camera);

INSTANTIATE_COMPONENTS_MANAGER_FUNCTIONS(Mesh);
INSTANTIATE_COMPONENTS_MANAGER_FUNCTIONS(Terrain);
INSTANTIATE_COMPONENTS_MANAGER_FUNCTIONS(CubicPatch);

INSTANTIATE_COMPONENTS_MANAGER_FUNCTIONS(DirectLight);
INSTANTIATE_COMPONENTS_MANAGER_FUNCTIONS(PointLight);
INSTANTIATE_COMPONENTS_MANAGER_FUNCTIONS(SpotLight);
