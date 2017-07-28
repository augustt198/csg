#pragma once

#include <vector>
#include <ostream>

#include "common.h"

namespace csg {

class Mesh {
    public:

    Mesh();

    void render();

    void updateVertices(std::vector<Vertex> *newVertices);

    void writeSTL(std::ostream& out, const char *solidName="mesh", bool ascii=true);

    void tryUpdate();

    private:
    GLuint VAO, VBO;
    std::vector<Vertex> *currentVertices;
    std::vector<Vertex> *newVertices;
    bool needsUpdate;
};

}
