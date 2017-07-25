#include "common.h"

#include <cmath>
#include <algorithm>

#define EPSILON 0.00001

namespace csg {

Vec3 lerp(float w, float val1, float val2, Vec3 v1, Vec3 v2) {
    if (std::abs(w - val1) < EPSILON)
        return v1;
    else if (std::abs(w - val2) < EPSILON)
        return v2;
    else if (std::abs(val1 - val2) < EPSILON)
        return v1;

    float mu = (w - val1) / (val2 - val1);
    return Vec3(
        v1.x + mu*(v2.x - v1.x),
        v1.y + mu*(v2.y - v1.y),
        v1.z + mu*(v2.z - v1.z)
    );
}

BoundingBox bbUnion(BoundingBox bb1, BoundingBox bb2) {
    Vec3 newMin(
        std::min(bb1.min.x, bb2.min.x),
        std::min(bb1.min.y, bb2.min.y),
        std::min(bb1.min.z, bb2.min.z)
    );
    Vec3 newMax(
        std::max(bb1.max.x, bb2.max.x),
        std::max(bb1.max.y, bb2.max.y),
        std::max(bb1.max.z, bb2.max.z)
    );
    return BoundingBox(newMin, newMax);
}

}
