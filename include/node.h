#pragma once

#include "common.h"

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

static int globalIDCounter = 0;

class Node {
    public:

    Type type;
    int id;

    Node() {
        id = globalIDCounter++;
    }

    const char *idString(const char *fmt);
    
    // values < 1 are inside the surface
    // values > 1 are outside the surface
    virtual float evaluate(float x, float y, float z) = 0;
    virtual BoundingBox getBoundingBox() = 0;
};

// union of left and right (commutative)
class Union : public Node {
    public:
    Node *left, *right;

    Union();
    virtual float evaluate(float x, float y, float z);
    virtual BoundingBox getBoundingBox();
};

// intersection of left and right (commutative)
class Intersection : public Node {
    public:
    Node *left, *right;

    Intersection();
    virtual float evaluate(float x, float y, float z);
    virtual BoundingBox getBoundingBox();
};

// difference of left and right (not commutative)
class Difference : public Node {
    public:
    Node *left, *right;

    Difference();
    virtual float evaluate(float x, float y, float z);
    virtual BoundingBox getBoundingBox();
};

class Sphere : public Node {
    public:
    float ax1, ax2, ax3;
    bool isLinked;
    float prop1, prop2, prop3;

    Sphere();
    virtual float evaluate(float x, float y, float z);
    virtual BoundingBox getBoundingBox();
};

class Cube : public Node {
    public:
    float ax1, ax2, ax3;

    Cube();
    virtual float evaluate(float x, float y, float z);
    virtual BoundingBox getBoundingBox();
};

class Translate : public Node {
    public:
    float dx, dy, dz;
    Node *node;

    Translate();
    virtual float evaluate(float x, float y, float z);
    virtual BoundingBox getBoundingBox();
};

class Rotate : public Node {
    public:
    float r1, r2, r3;
    Node *node;

    Rotate();
    virtual float evaluate(float x0, float y0, float z0);
};
    
} // end namespace node
} // end namespace csg
