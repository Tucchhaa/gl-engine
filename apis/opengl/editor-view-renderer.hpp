#pragma once

#include "deferred-renderer.hpp"
#include "../base/ieditor-view-renderer.hpp"

class EditorViewRenderer: public DeferredRenderer, public IEditorViewRenderer {
private:
    Shader editorShader;

    GameObject* positionHandle = nullptr;

    vector<IRenderObject*> editorMeshes;

public:
    EditorViewRenderer();

    void render() override;

private:
    void beforeRender();

    void renderEditorTools();

protected:
    GameObject* createPositionHandle();
};
