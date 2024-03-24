#pragma once

#include "../../core/scene.hpp"

class IRenderer {
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
    virtual void setScreenSize(int width, int height) = 0;
};
