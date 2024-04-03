#include "cubic-patch.hpp"

CubicPatch::CubicPatch(
        vector<float> controlPoints, Material material,
        float tessOuterLevel, float tessInnerLevel
    ) :
        controlPoints(controlPoints), material(material),
        tessOuterLevel(tessOuterLevel), tessInnerLevel(tessInnerLevel)
    {}


int CubicPatch::getVerticesCount() const {
    return VERTICES_PER_PATCH * 1;
}