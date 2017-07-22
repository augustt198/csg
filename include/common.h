#pragma once

namespace csg {

struct Vec3 {
    float x, y, z;

    Vec3(float _x=0, float _y=0, float _z=0) : x(_x), y(_y), z(_z) {}
};

Vec3 lerp(float w, float val1, float val2, Vec3 v1, Vec3 v2);

}
