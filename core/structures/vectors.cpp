#include "vectors.hpp"

Vec3::Vec3(float x, float y, float z) : x(x), y(y), z(z)
    {}

Vec3::Vec3() : x(0), y(0), z(0)
    {}

// ===

Vec2::Vec2(float x, float y) : x(x), y(y)
    {}

Vec2::Vec2() : x(0), y(0)
    {}

Vec3& Vec3::operator=(const Vec3& other) = default;