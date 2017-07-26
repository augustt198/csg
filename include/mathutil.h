#pragma once

namespace csg {

float clamp(float a, float lower, float upper);

float mix(float a, float b, float t);

// thanks to Inigo Quilez
float smoothmin(float a, float b, float k=0.05);

// min(x, y) = -max(-x, -y)
float smoothmax(float a, float b, float k=0.05);

float GLOBAL_MAX_FUNC(float a, float b);
float GLOBAL_MIN_FUNC(float a, float b);

}