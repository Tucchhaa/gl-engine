#include "scene.hpp"

#include "./hierarchy.hpp"
#include "./game-object.hpp"

Scene::Scene(Loader* loader): loader(loader) {}

// ===
// Getters
// ===
Camera* Scene::getCamera() const {
    return camera;
}

void Scene::setCamera(Camera* camera) {
    this->camera = camera;
}

const vector<Mesh*>& Scene::getMeshes() {
    return meshes;
}

const vector<Terrain*>& Scene::getTerrains() {
    return terrains;
}

const vector<CubicPatch*> &Scene::getCubicPatches() {
    return cubicPatches;
}

const vector<PointLight*>& Scene::getPointLights() {
    return pointLights;
}

const vector<DirectLight*>& Scene::getDirectLights() {
    return directLights;
}

const vector<SpotLight*> &Scene::getSpotLights() {
    return spotLights;
}

// ===
// Events
// ===

void Scene::setupScene() {
    processHierarchy();

    Hierarchy::updateTransformTree();
}

void Scene::beforeRender() {
    Hierarchy::updateTransformTree(camera->gameObject);
}

void Scene::afterRender() { }

void Scene::finish() { }

// ===

void Scene::processHierarchy() {
    loadComponents(&meshes);
    loadComponents(&terrains);
    loadComponents(&cubicPatches);
    loadComponents(&directLights);
    loadComponents(&pointLights);
    loadComponents(&spotLights);
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
