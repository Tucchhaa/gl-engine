#pragma once

#include "irender-object.hpp"

#include "../../core/scene.hpp"

class IRenderer: public IGameEventsListener {
protected:
    int frameWidth = 1280;
    int frameHeight = 720;

    Scene* currentScene = nullptr;

    vector<IRenderObject*> meshes;

    map<const Material*, vector<IRenderObject*>> meshesByMaterial;

    vector<IRenderObject*> terrains;

    vector<IRenderObject*> cubicPatches;

public:
    virtual ~IRenderer() = default;

    void afterSceneSetup() override;
    
    /**
     * Renders current scene
     */
    virtual void render() = 0;
    
    /**
     *  Updates size of frame
     */
    virtual void setFrameSize(int width, int height);
};
