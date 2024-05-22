#include "component.hpp"


ObjectComponent::ObjectComponent() {
    ComponentID = generateComponentId();
}

void ObjectComponent::onGameObjectSet() { }

// ===
// implement IGameEventsListener
// ===
void ObjectComponent::afterSceneSetup() { }

void ObjectComponent::beforeRender() { }

void ObjectComponent::afterRender() { }
