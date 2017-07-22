#include "common.h"

#include <cmath>

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

}
