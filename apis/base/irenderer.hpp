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
    virtual void setScreenSize(unsigned int width, unsigned int height) = 0;
};
