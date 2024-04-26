#pragma once

#include <vector>

#include "loader.hpp"
#include "./components/camera.hpp"

#include "./components/mesh.hpp"
#include "./components/terrain.hpp"
#include "./components/cubic-patch.hpp"

#include "./components/lights/direct-light.hpp"
#include "./components/lights/point-light.hpp"
#include "./components/lights/spot-light.hpp"

class Scene {
protected:
    Loader* loader;

    Camera* camera = nullptr;

private:
    vector<Mesh*> meshes;

    vector<Terrain*> terrains;

    vector<CubicPatch*> cubicPatches;
    
    vector<DirectLight*> directLights;
    
    vector<PointLight*> pointLights;

    vector<SpotLight*> spotLights;


public:
    explicit Scene(Loader* loader);

    virtual ~Scene() = default;

// ===
// Getters
// ===
public:
    Camera* getCamera() const;
    
    void setCamera(Camera* camera);

    const vector<Mesh*>& getMeshes();

    const vector<Terrain*>& getTerrains();

    const vector<CubicPatch*>& getCubicPatches();

    const vector<DirectLight*>& getDirectLights();

    const vector<PointLight*>& getPointLights();

    const vector<SpotLight*>& getSpotLights();

// ===
// Events
// ===
public:

    virtual void beforeRender();

    virtual void afterRender();

    virtual void setupScene();

    virtual void finish();

protected:
    virtual void processHierarchy();

private:
    template<typename T>
    void loadComponents(vector<T*>* array);
};
