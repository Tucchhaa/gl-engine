#include "scene.hpp"

Scene::Scene() = default;

Camera* Scene::getCamera() {
    return camera;
}

void Scene::setCamera(Camera* camera) {
    this->camera = camera;
}

const vector<Mesh*>& Scene::getMeshes() {
    return meshes;
}

const vector<PointLight*>& Scene::getPointLights() {
    return pointLights;
}

const vector<DirectLight*>& Scene::getDirectLights() {
    return directLights;
}

void Scene::processHierarchy() {
    loadComponents(&meshes);
    loadComponents(&directLights);
    loadComponents(&pointLights);
}

template<typename T>
void Scene::loadComponents(vector<T*>* array) {
    array->clear();
    
    map<int, GameObject*>* gameObjects = Hierarchy::getGameObjects();
    
    map<int, GameObject*>::iterator iterator;
    
    for(iterator = gameObjects->begin(); iterator != gameObjects->end(); iterator++) {
        int objectId = iterator->second->ID;
        
        vector<T*> components = Hierarchy::Components<T>::getAll(objectId);
        
        array->insert(array->end(), components.begin(), components.end());
    }
}
