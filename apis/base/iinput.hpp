#pragma once

#include <glm/glm.hpp>

class IInput {
protected:
    static IInput* instance;

public:
    virtual ~IInput() = default;

    static IInput* getInstance();

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
