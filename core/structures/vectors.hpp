#pragma once

struct Vec3 {
public:
    float x;
    float y;
    float z;

    Vec3();

    Vec3(float x, float y, float z);

public:
    Vec3 &operator=(const Vec3 &other);
};

struct  Vec2 {
public:
    float x;
    float y;

    Vec2();

    Vec2(float x, float z);
};