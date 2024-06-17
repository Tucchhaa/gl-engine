#pragma once

#include <glm/glm.hpp>

class IInput {
public:
    IInput() = default;

    virtual ~IInput() = default;

    // ===

    /**
     * Reset input state
     */
    virtual void reset() = 0;

    virtual void calculateDeltaTime() = 0;

    /**
     * Process input events
     */
    virtual void process() = 0;

    // ===

    virtual glm::vec2 axisVec2() = 0;

    /**
     *
     * @return 1.0 if D is pressed, -1.0 if A is pressed, 0 otherwise
     */
    virtual float axisHorizontal() = 0;

    /**
     *
     * @return 1.0 if W is pressed, -1.0 if S is pressed, 0 otherwise
     */
    virtual float axisVertical() = 0;

    virtual glm::vec2 mouseDelta() = 0;

    // ===

    virtual bool isShiftPressed() = 0;

    virtual bool isSpacePressed() = 0;

    // ===

    virtual bool isLeftMousePressed() = 0;

    virtual float getDeltaTime() = 0;
};
