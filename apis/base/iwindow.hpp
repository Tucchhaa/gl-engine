#pragma once

#include <iostream>

class IWindow {
public:
    virtual ~IWindow() = default;

    virtual void create(int screenWidth, int screenHeight) = 0;

    virtual bool isOpen() = 0;

    virtual void onRendered() = 0;

    virtual void terminate() = 0;
};
