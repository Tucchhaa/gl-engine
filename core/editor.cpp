#include "editor.hpp"

#include "../apis/base/ieditor-view-renderer.hpp"
#include "../apis/base/iengine.hpp"
#include "../demos/backpack-demo.hpp"

void Editor::afterSceneSetup() {
    currentScene = IEngine::CurrentScene;

    const auto demo = dynamic_cast<BackpackDemo*>(currentScene);

    const auto editorRenderer = dynamic_cast<IEditorViewRenderer*>(IEngine::Renderer);

    // editorRenderer->selectObject(demo->backpack);
}
