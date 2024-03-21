#include "scene.hpp"

Scene::Scene() { }

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
    loadComponents();
}

void Scene::loadComponents() {
    this->meshes.clear();
    this->directLights.clear();
    this->pointLights.clear();
    
    map<int, GameObject*>* gameObjects = Hierarchy::getGameObjects();
    
    map<int, GameObject*>::iterator iterator;
    
    for(iterator = gameObjects->begin(); iterator != gameObjects->end(); iterator++) {
        int objectId = iterator->second->ID;
        
        vector<Mesh*> meshes = Hierarchy::getComponents<Mesh>(objectId);
        vector<DirectLight*> directLights = Hierarchy::getComponents<DirectLight>(objectId);
        vector<PointLight*> pointLights = Hierarchy::getComponents<PointLight>(objectId);
        
        this->meshes.insert(this->meshes.end(), meshes.begin(), meshes.end());
        this->directLights.insert(this->directLights.end(), directLights.begin(), directLights.end());
        this->pointLights.insert(this->pointLights.end(), pointLights.begin(), pointLights.end());
    }
}

