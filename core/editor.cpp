#include "editor.hpp"

#include "../apis/base/ieditor-view-renderer.hpp"
#include "../apis/base/iengine.hpp"
#include "../demos/backpack-demo.hpp"

void Editor::setScene(Scene* scene) {
    currentScene = scene;

    const auto demo = dynamic_cast<BackpackDemo*>(scene);

    const auto editorRenderer = dynamic_cast<IEditorViewRenderer*>(IEngine::Renderer);

    editorRenderer->selectObject(demo->backpack);
}

