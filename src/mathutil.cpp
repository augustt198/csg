#include "mathutil.h"

#include <algorithm>

namespace csg {

bool GLOBAL_SMOOTHING_ENABLED = false;
float GLOBAL_SMOOTHING_CONSTANT = 0.1;

float clamp(float a, float lower, float upper) {
    if (a < lower)
        return lower;
    if (a > upper)
        return upper;
    return a;
}

float mix(float a, float b, float t) {
    return a + t*(b - a);
}

// thanks to Inigo Quilez
float smoothmin(float a, float b, float k){ 
    float h = clamp(0.5 + 0.5*(b - a)/k, 0.0, 1.0);
    return mix(b, a, h) - k*h*(1.0 - h);
}

// min(x, y) = -max(-x, -y)
float smoothmax(float a, float b, float k) {
    return -smoothmin(-a, -b, k);
}


float GLOBAL_MAX_FUNC(float a, float b) {
    if (GLOBAL_SMOOTHING_ENABLED) {
        return smoothmax(a, b, GLOBAL_SMOOTHING_CONSTANT);
    } else {
        return std::max(a, b);
    }
}
float GLOBAL_MIN_FUNC(float a, float b) {
    if (GLOBAL_SMOOTHING_ENABLED) {
        return smoothmin(a, b, GLOBAL_SMOOTHING_CONSTANT);
    } else {
        return std::min(a, b);
    }
}

}