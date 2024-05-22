#include "igame-events-listener.hpp"

#include "../apis/base/iengine.hpp"

IGameEventsListener::IGameEventsListener() {
    IEngine::gameEventsListeners.push_back(this);
}

void IGameEventsListener::afterSceneSetup() { }

void IGameEventsListener::beforeRender() { }

void IGameEventsListener::afterRender() { }
