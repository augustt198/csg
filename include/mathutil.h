#pragma once

namespace csg {

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
float smoothmin(float a, float b, float k=0.05){ 
    float h = clamp(0.5 + 0.5*(b - a)/k, 0.0, 1.0);
    return mix(b, a, h) - k*h*(1.0 - h);
}

// min(x, y) = -max(-x, -y)
float smoothmax(float a, float b, float k=0.05) {
    return -smoothmin(-a, -b, k);
}

}