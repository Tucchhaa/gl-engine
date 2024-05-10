#pragma once

class IWindow {
public:
    int screenWidth = 0;
    int screenHeight = 0;

    virtual ~IWindow() = default;

    virtual void create(int screenWidth, int screenHeight);

    virtual bool isOpen() = 0;

    virtual void onRendered() = 0;

    virtual void terminate() = 0;
};
