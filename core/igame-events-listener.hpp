#pragma once

class IGameEventsListener {
public:
    IGameEventsListener();

    virtual ~IGameEventsListener() = default;

    virtual void setupScene() = 0;

    virtual void beforeRender() = 0;

    virtual void afterRender() = 0;
};
