#include "editor-view-renderer.hpp"

#include "render-object.hpp"

EditorViewRenderer::EditorViewRenderer(): DeferredRenderer(), IEditorViewRenderer(),
    editorShader("editor/mesh.vert", "editor/mesh.frag")
{
    positionHandle = createPositionHandle();
}

void EditorViewRenderer::render() {
    beforeRender();

    DeferredRenderer::render();

    renderEditorTools();
}

void EditorViewRenderer::beforeRender() {
    positionHandle->transform->setPosition(selectedObject->transform->getPosition());

    Hierarchy::updateTransformTree(positionHandle);
}

void EditorViewRenderer::renderEditorTools() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glClear(GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_STENCIL_TEST);

    const mat4 viewProjection = currentScene->getCamera()->getViewProjectionMatrix();

    editorShader.use();
    editorShader.setMat4("perspective", viewProjection);

    for(auto* object: editorMeshes) {
        const auto* mesh = object->getMesh<Mesh>();

        const Vec3* color = (*mesh->gameObject->parent->getDataList<Vec3>())[0];
        const Transform* transform = object->getMesh<Mesh>()->transform;

        editorShader.setVec3("color", *color);
        editorShader.setMat4("transform", transform->getTransformMatrix());

        object->render();
    }
}

GameObject* EditorViewRenderer::createPositionHandle() {
    vector<Mesh*> handleMeshes;

    GameObject* result = IEditorViewRenderer::createPositionHandle(&handleMeshes);

    for(const auto mesh: handleMeshes) {
        const auto object = new RenderObject(mesh);

        editorMeshes.push_back(object);
    }

    return result;
}
