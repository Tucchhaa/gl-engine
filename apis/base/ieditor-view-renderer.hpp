#pragma once

#include "../../core/components/mesh.hpp"
#include "../../core/game-object.hpp"

class IEditorViewRenderer {
protected:
    GameObject* selectedObject = nullptr;

public:
    IEditorViewRenderer() = default;

    virtual ~IEditorViewRenderer() = default;

    void selectObject(GameObject* object);

protected:
    static GameObject* createPositionHandle(vector<Mesh*>* handleMeshes);
};
