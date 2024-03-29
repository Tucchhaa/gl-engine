#pragma once

#include <vector>
#include <map>

#include "./hierarchy.hpp"

#include "./game-object/game-object.hpp"
#include "./game-object/components/camera.hpp"
#include "./game-object/components/mesh.hpp"

#include "./game-object/components/lights/direct-light.hpp"
#include "./game-object/components/lights/point-light.hpp"
#include "./game-object/components/lights/spot-light.hpp"

class Scene {
private:
    Camera* camera;

    vector<Mesh*> meshes;
    
    vector<DirectLight*> directLights;
    
    vector<PointLight*> pointLights;

    vector<SpotLight*> spotLights;
    
public:
    Scene();
    
    Camera* getCamera();
    
    void setCamera(Camera* camera);
    
    const vector<Mesh*>& getMeshes();

    const vector<DirectLight*>& getDirectLights();

    const vector<PointLight*>& getPointLights();

    const vector<SpotLight*>& getSpotLights();

    void processHierarchy();
    
private:
    template<typename T>
    void loadComponents(vector<T*>* array);
};
