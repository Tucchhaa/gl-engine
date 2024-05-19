#include "igame-events-listener.hpp"

#include "../apis/base/iengine.hpp"

IGameEventsListener::IGameEventsListener() {
    IEngine::gameEventsListeners.push_back(this);
}
