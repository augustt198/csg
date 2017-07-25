#include "node.h"

#include <cmath>
#include <algorithm>

#include "mathutil.h"

using std::sin;
using std::cos;
using std::max;
using std::abs;

namespace csg {
namespace node {

const char *Node::idString(const char *fmt) {
    char *res;
    asprintf(&res, fmt, id);
    return res;
}


Union::Union() : left(NULL), right(NULL) {
    type = CSG_UNION;
}

float Union::evaluate(float x, float y, float z) {
    return smoothmin(left->evaluate(x, y, z), right->evaluate(x, y, z));
}

BoundingBox Union::getBoundingBox() {
    return bbUnion(left->getBoundingBox(), right->getBoundingBox());
}


Intersection::Intersection() : left(NULL), right(NULL) {
    type = CSG_INTERSECTION;
}

float Intersection::evaluate(float x, float y, float z) {
    return smoothmax(left->evaluate(x, y, z), right->evaluate(x, y, z));
}

// TODO
BoundingBox Intersection::getBoundingBox() {
    return BoundingBox(Vec3(), Vec3());
}


Difference::Difference() : left(NULL), right(NULL) {
    type = CSG_DIFFERENCE;
}

float Difference::evaluate(float x, float y, float z) {
    return smoothmax(left->evaluate(x, y, z), -right->evaluate(x, y, z));
}

// can't make any assumptions
BoundingBox Difference::getBoundingBox() {
    return bbUnion(left->getBoundingBox(), right->getBoundingBox());
}


Sphere::Sphere() {
    type = CSG_SPHERE;
    ax1 = 1.0, ax2 = 1.0, ax3 = 1.0;
    isLinked = true;
}

float Sphere::evaluate(float x, float y, float z) {
    return x*x/(ax1*ax1) + y*y/(ax2*ax2) + z*z/(ax3*ax3) - 1;
}

BoundingBox Sphere::getBoundingBox() {
    return BoundingBox(
        Vec3(-ax1, -ax2, -ax3),
        Vec3(ax1, ax2, ax3)
    );
}


Cube::Cube() {
    type = CSG_CUBE;
    ax1 = 0.5, ax2 = 0.5, ax3 = 0.5;
}

float Cube::evaluate(float x, float y, float z) {
    return max(max(abs(x), abs(y)), abs(z)) - 0.5;
}

BoundingBox Cube::getBoundingBox() {
    return BoundingBox(
        Vec3(-ax1, -ax2, -ax3),
        Vec3(ax1, ax2, ax3)
    );
}


Translate::Translate() {
    type = CSG_TRANSLATE;
    dx = 0.0, dy = 0.0, dz = 0.0;
    isLinked = false;
    node = NULL;
}

float Translate::evaluate(float x, float y, float z) {
    return node->evaluate(x-dx, y-dx, z-dx);
}

BoundingBox Translate::getBoundingBox() {
    BoundingBox bb = node->getBoundingBox();
    return BoundingBox(
        Vec3(bb.min.x+dx, bb.min.y+dy, bb.min.z+dy),
        Vec3(bb.max.x+dx, bb.max.y+dy, bb.max.z+dy)
    );
}


Rotate::Rotate() {
    type = CSG_ROTATE;
    r1 = 0.0, r2 = 0.0, r3 = 0.0;
    node = NULL;
}

float Rotate::evaluate(float x0, float y0, float z0) {
    float s1 = sin(r1), c1 = cos(r1),
            s2 = sin(r2), c2 = cos(r2),
            s3 = sin(r3), c3 = cos(r3);

    float x = x0*(c2*c3) - y0*s2 + z0*(c2*s3);
    float y = x0*(s1*s3+c1*c3*s2) + y0*(c1*c2) + z0*(c1*s2*s3 - c3*s1);
    float z = z0*(c3*s1*s2 - c1*s3) + y0*(c2*s1) + z0*(c1*c3 + s1*s2*s3);

    return node->evaluate(x, y, z);
}

Scale::Scale() {
    type = CSG_SCALE;
    sx = 1.0, sy = 1.0, sz = 1.0;
    isLinked = true;
    node = NULL;
}

float Scale::evaluate(float x, float y, float z) {
    return node->evaluate(x/sx, y/sy, z/sy);
}

BoundingBox Scale::getBoundingBox() {
    BoundingBox bb = node->getBoundingBox();
    return BoundingBox(
        Vec3(bb.min.x*sx, bb.min.y*sy, bb.min.z*sz),
        Vec3(bb.max.x*sx, bb.max.y*sy, bb.max.z*sz)
    );
}

}
}