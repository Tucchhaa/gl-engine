#include "ieditor-view-renderer.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../../core/scene.hpp"
#include "../../core/hierarchy.hpp"
#include "../../core/structures/vectors.hpp"

using namespace glm;
using namespace std;

void IEditorViewRenderer::selectObject(GameObject* object) {
    selectedObject = object;
}

GameObject* IEditorViewRenderer::createPositionHandle(vector<Mesh*>* handleMeshes) {
    auto* positionHandle = new GameObject();

    GameObject* axisXCylinder = Scene::createCylinder();
    GameObject* axisYCylinder = Scene::createCylinder();
    GameObject* axisZCylinder = Scene::createCylinder();

    GameObject* axisXCone = Scene::createCone();
    GameObject* axisYCone = Scene::createCone();
    GameObject* axisZCone = Scene::createCone();

    float handleScale = 0.05;

    axisXCylinder->addDataItem(new Vec3(1.0, 0.0, 0.0));
    axisXCylinder->transform->setScale(handleScale, 1.0, handleScale);
    axisXCylinder->transform->rotate(quat(vec3(0.0, 0.0, radians(-90.0))));
    axisXCylinder->transform->setPosition(1.0, 0.0, 0.0);

    axisYCylinder->addDataItem(new Vec3(0.0, 1.0, 0.0));
    axisYCylinder->transform->setScale(handleScale, 1.0, handleScale);
    axisYCylinder->transform->setPosition(0, 1.0, 0);

    axisZCylinder->addDataItem(new Vec3(0.0, 0.0, 1.0));
    axisZCylinder->transform->setScale(handleScale, 1.0, handleScale);
    axisZCylinder->transform->rotate(quat(glm::vec3(radians(90.0), 0.0, 0)));
    axisZCylinder->transform->setPosition(0.0, 0.0, 1.0);

    axisXCone->addDataItem(new Vec3(1.0, 0.0, 0.0));
    axisXCone->transform->setScale(2*handleScale);
    axisXCone->transform->setPosition(2.0, 0.0, 0.0);
    axisXCone->transform->rotate(quat(vec3(0.0, 0.0, radians(-90.0))));

    axisYCone->addDataItem(new Vec3(0.0, 1.0, 0.0));
    axisYCone->transform->setScale(2*handleScale);
    axisYCone->transform->setPosition(0.0, 2.0, 0.0);

    axisZCone->addDataItem(new Vec3(0.0, 0.0, 1.0));
    axisZCone->transform->setScale(2*handleScale);
    axisZCone->transform->setPosition(0.0, 0.0, 2.0);
    axisZCone->transform->rotate(quat(vec3(radians(90.0), 0.0, 0)));

    const vector children = { axisXCylinder, axisYCylinder, axisZCylinder, axisXCone, axisYCone, axisZCone };

    Hierarchy::setParent(children, positionHandle);
    Hierarchy::updateTransformTree(positionHandle);

    for(GameObject* object: children) {
        Mesh* mesh = object->components.getAllFromChildren<Mesh>()[0];

        handleMeshes->push_back(mesh);
    }

    return positionHandle;
}