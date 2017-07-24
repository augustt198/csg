#pragma once

#include <cstdio>
#include <vector>

#include "common.h"

namespace csg {

class Mesh {
    public:

    Mesh(std::vector<Vertex> vertices) : vertices(vertices) {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &(vertices[0]), GL_DYNAMIC_DRAW);

        Vertex test;
        std::printf("test thingy %ld\n", (char*)&test.position - (char*)&test);
        std::printf("test thingy2 %ld\n", (char*)&test.normal - (char*)&test);
        std::printf("test thingy 3 %ld\n", sizeof(Vec3));

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), (void*) 0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(Vertex), (void*) sizeof(Vec3));

        glBindVertexArray(0);
    }

    void render() {
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());
        glBindVertexArray(0);
    }

    void updateVertices(std::vector<Vertex> newVertices) {
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        if (newVertices.size() < vertices.size()) {
            glBufferSubData(GL_ARRAY_BUFFER, 0, newVertices.size() * sizeof(Vertex), &(newVertices[0]));
        } else {
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &(vertices[0]), GL_DYNAMIC_DRAW);
        }

        vertices = newVertices;
    }

    private:
    GLuint VAO, VBO;
    std::vector<Vertex> vertices;
};

}
