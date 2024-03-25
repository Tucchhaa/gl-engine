#pragma once

#include <glm/glm.hpp>

class IInput {
public:
    virtual ~IInput() = default;

    // ===

    virtual void process() = 0;

    // ===

    virtual glm::vec3 axisVec3() = 0;

    virtual float axisHorizontal() = 0;

    virtual float axisVertical() = 0;

    // ===

    virtual bool isShiftPressed() = 0;

    virtual float getDeltaTime() = 0;
};
