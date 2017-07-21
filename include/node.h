#pragma once

namespace csg {
namespace node {

enum Type {
    CSG_UNION,
    CSG_INTERSECTION,
    CSG_DIFFERENCE,
    CSG_SPHERE,
    CSG_CUBE
};

class Node {
    public:

    Type type;
};

class Union : public Node {
    public:
    Node *left, *right;

    Union() {
        type = CSG_UNION;
    }
};

class Intersection : public Node {
    public:
    Node *left, *right;

    Intersection() {
        type = CSG_INTERSECTION;
    }
};

class Difference : public Node {
    public:
    Node *left, *right;

    Difference() {
        type = CSG_DIFFERENCE;
    }
};

class Sphere : public Node {
    public:
    float ax1, ax2, ax3;

    Sphere() {
        type = CSG_SPHERE;
    }
};

class Cube : public Node {
    public:
    float ax1, ax2, ax3;

    Cube() {
        type = CSG_SPHERE;
    }
};

} // end namespace node
} // end namespace csg
