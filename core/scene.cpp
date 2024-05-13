#include "scene.hpp"

#include "./hierarchy.hpp"
#include "./game-object.hpp"
#include "../apis/base/iengine.hpp"

Scene::Scene() {
    loader = IEngine::Loader;
    input = IEngine::Input;
}

Scene::~Scene() = default;

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

const map<const Material*, vector<Mesh*>> Scene::getMeshesByMaterial() const {
    return meshesByMaterial;
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

    meshesByMaterial = processMeshesByMaterial();
}

map<const Material*, vector<Mesh*>> Scene::processMeshesByMaterial() const {
    map<const Material*, vector<Mesh*>> result;

    for(auto* mesh: meshes) {
        result[mesh->material].push_back(mesh);
    }

    return result;
}

template<typename T>
void Scene::loadComponents(vector<T*>* array) {
    array->clear();

    map<int, GameObject*>* gameObjects = Hierarchy::getGameObjects();
    
    map<int, GameObject*>::iterator iterator;
    
    for(iterator = gameObjects->begin(); iterator != gameObjects->end(); iterator++) {
        GameObject* gameObject = iterator->second;
        
        vector<T*> components = gameObject->components.getAll<T>();
        
        array->insert(array->end(), components.begin(), components.end());
    }
}

// ===
// Basic shapes
// ===

GameObject* Scene::createCube() {
    return IEngine::Loader->loadModel("models/shapes/cube.obj");
}

GameObject* Scene::createSphere() {
    return IEngine::Loader->loadModel("models/shapes/sphere.obj");
}

GameObject* Scene::createPlane() {
    return IEngine::Loader->loadModel("models/shapes/plane.obj");
}

GameObject* Scene::createCylinder() {
    return IEngine::Loader->loadModel("models/shapes/cylinder.obj");
}

GameObject* Scene::createCone() {
    return IEngine::Loader->loadModel("models/shapes/cone.obj");
}