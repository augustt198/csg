#pragma once

#include <cmath>
#include <algorithm>

using std::sin;
using std::cos;

namespace csg {
namespace node {

enum Type {
    CSG_NONE,
    CSG_UNION,
    CSG_INTERSECTION,
    CSG_DIFFERENCE,
    CSG_SPHERE,
    CSG_CUBE,
    CSG_TRANSLATE,
    CSG_ROTATE
};

class Node {
    public:

    Type type;
    
    // values < 1 are inside the surface
    // values > 1 are outside the surface
    virtual float evaluate(float x, float y, float z) = 0;
};

// union of left and right (commutative)
class Union : public Node {
    public:
    Node *left, *right;

    Union() {
        type = CSG_UNION;
        left = NULL, right = NULL;
    }

    virtual float evaluate(float x, float y, float z) {
        return std::min(left->evaluate(x, y, z), right->evaluate(x, y, z));
    }
};

// intersection of left and right (commutative)
class Intersection : public Node {
    public:
    Node *left, *right;

    Intersection() {
        type = CSG_INTERSECTION;
        left = NULL, right = NULL;
    }

    virtual float evaluate(float x, float y, float z) {
        return std::max(left->evaluate(x, y, z), right->evaluate(x, y, z));
    }
};

// difference of left and right (not commutative)
class Difference : public Node {
    public:
    Node *left, *right;

    Difference() {
        type = CSG_DIFFERENCE;
        left = NULL, right = NULL;
    }

    virtual float evaluate(float x, float y, float z) {
        return std::max(left->evaluate(x, y, z), -right->evaluate(x, y, z));
    }
};

class Sphere : public Node {
    public:
    float ax1, ax2, ax3;
    bool isLinked;
    float prop1, prop2, prop3;

    Sphere() {
        type = CSG_SPHERE;
        ax1 = 1.0, ax2 = 1.0, ax3 = 1.0;
        isLinked = true;
        prop1 = 1.0, prop2 = 1.0, prop3 = 1.0;
    }

    virtual float evaluate(float x, float y, float z) {
        return x*x/(ax1*ax1) + y*y/(ax2*ax2) + z*z/(ax3*ax3);
    }
};

class Cube : public Node {
    public:
    float ax1, ax2, ax3;

    Cube() {
        type = CSG_CUBE;
        ax1 = 1.0, ax2 = 1.0, ax3 = 1.0;
    }

    virtual float evaluate(float x, float y, float z) {
        if (x>=-ax1 && x<=ax1 && y>=-ax2 && y<=ax2 && y>=-ax3 && y>=ax3) {
            return 0.5;
        } else {
            return 1.5;
        }
    }
};

class Translate : public Node {
    public:
    float dx, dy, dz;
    Node *node;

    Translate() {
        type = CSG_TRANSLATE;
        dx = 0.0, dy = 0.0, dz = 0.0;
    }

    virtual float evaluate(float x, float y, float z) {
        return node->evaluate(x-dx, y-dx, z-dx);
    }
};

class Rotate : public Node {
    public:
    float r1, r2, r3;
    Node *node;

    Rotate() {
        type = CSG_ROTATE;
        r1 = 0.0, r2 = 0.0, r3 = 0.0;
    }

    virtual float evaluate(float x0, float y0, float z0) {
        float s1 = sin(r1), c1 = cos(r1),
              s2 = sin(r2), c2 = cos(r2),
              s3 = sin(r3), c3 = cos(r3);

        float x = x0*(c2*c3) - y0*s2 + z0*(c2*s3);
        float y = x0*(s1*s3+c1*c3*s2) + y0*(c1*c2) + z0*(c1*s2*s3 - c3*s1);
        float z = z0*(c3*s1*s2 - c1*s3) + y0*(c2*s1) + z0*(c1*c3 + s1*s2*s3);

        return node->evaluate(x, y, z);
    }
};
    
} // end namespace node
} // end namespace csg
