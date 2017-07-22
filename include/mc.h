#pragma once

#include <vector>

#include "common.h"

namespace csg {
namespace mc {

typedef float(*ScalarFieldFunc)(float, float, float);

class MarchingCubes {
    public:
    float minX, minY, minZ, maxX, maxY, maxZ;
    float stepX, stepY, stepZ;

    MarchingCubes(float _minX, float _minY, float _minZ, float _maxX,
        float _maxY, float _maxZ, float _stepX, float _stepY, float _stepZ) :
        minX(_minX), minY(_minY), minZ(_minZ), maxX(_maxX), maxY(_maxY), maxZ(_maxZ),
        stepX(_stepX), stepY(_stepY), stepZ(_stepZ) {}

    
    std::vector<Vec3> *isosurface(ScalarFieldFunc f, float w);
     
};

}
}
