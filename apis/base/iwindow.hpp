#pragma once

class IWindow {
public:
    int screenWidth = 0;
    int screenHeight = 0;

    virtual ~IWindow() = default;

    virtual void create(int screenWidth, int screenHeight);

    virtual bool isOpen() = 0;

    virtual void pollEvents() = 0;

    virtual void terminate() = 0;

    virtual void getFrameBufferSize(int &width, int &height) = 0;
};
