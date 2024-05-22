#pragma once

class IGameEventsListener {
public:
    IGameEventsListener();

    virtual ~IGameEventsListener() = default;

    virtual void afterSceneSetup();

    virtual void beforeRender();

    virtual void afterRender();
};
