#include "cubic-patch.hpp"

CubicPatch::CubicPatch(
        vector<float> controlPoints, Material material,
        float tessOuterLevel, float tessInnerLevel
    ) :
        controlPoints(controlPoints), material(material),
        tessOuterLevel(tessOuterLevel), tessInnerLevel(tessInnerLevel)
    {}


// === IMesh implementation ===

void* CubicPatch::data() {
    return controlPoints.data();
}

int CubicPatch::dataSize() {
    return static_cast<int>(controlPoints.size()) * sizeof(float);
}

vector<int> CubicPatch::shaderAttributes() {
    return {3};
}

bool CubicPatch::usesTessellation() {
    return true;
}

int CubicPatch::getVertexCount() {
    return VERTICES_PER_PATCH;
}

int CubicPatch::getVerticesPerPatch() {
    return VERTICES_PER_PATCH;
}
