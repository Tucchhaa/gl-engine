#pragma once

#include "irender-object.hpp"

#include "../../core/scene.hpp"

class IRenderer {
protected:
    int screenWidth = 2000;
    int screenHeight = 1600;

    Scene* currentScene = nullptr;

    vector<IRenderObject*> meshes;

    vector<IRenderObject*> terrains;

    vector<IRenderObject*> cubicPatches;

public:
    virtual ~IRenderer() = default;
    
    virtual void setScene(Scene* scene) = 0;
    
    /**
     * Renders current scene
     */
    virtual void render() = 0;
    
    /**
     *  Updates size of frame
     */
    virtual void setScreenSize(int width, int height);
};
