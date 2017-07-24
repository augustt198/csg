#pragma once

#include <vector>

#include "common.h"

namespace csg {

class Mesh {
    public:

    Mesh(std::vector<Vertex> vertices);

    void render();

    void updateVertices(std::vector<Vertex> newVertices);

    private:
    GLuint VAO, VBO;
    std::vector<Vertex> vertices;
};

}
