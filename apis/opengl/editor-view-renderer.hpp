#pragma once

#include "deferred-renderer.hpp"

class EditorViewRenderer: public DeferredRenderer {
private:
    Shader editorShader;

    GameObject* selectedObject = nullptr;

    GameObject* positionHandle = nullptr;

    vector<IRenderObject*> editorMeshes;

public:
    EditorViewRenderer();

    void render() override;

private:
    static GameObject* createPositionHandle(vector<IRenderObject*>& editorMeshes);

    void renderEditorTools();
};
