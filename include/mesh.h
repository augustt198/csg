#pragma once

#include <vector>
#include <ostream>

#include "common.h"

namespace csg {

class Mesh {
    public:

    Mesh(std::vector<Vertex> vertices);

    void render();

    void updateVertices(std::vector<Vertex> newVertices);

    void writeSTL(std::ostream& out, const char *solidName="mesh", bool ascii=true);

    private:
    GLuint VAO, VBO;
    std::vector<Vertex> vertices;
};

}
