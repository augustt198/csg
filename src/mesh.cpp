#include "mesh.h"

namespace csg {

Mesh::Mesh(std::vector<Vertex> vertices) : vertices(vertices) {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &(vertices[0]), GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    // the vertex positions - no offset b/c it's first in struct
    glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), (void*) 0);
    glEnableVertexAttribArray(1);
    // the normal vectors - offset by the vertex positions (Vec3)
    glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(Vertex), (void*) sizeof(Vec3));

    glBindVertexArray(0);
}

void Mesh::render() {
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    glBindVertexArray(0);
}


void Mesh::updateVertices(std::vector<Vertex> newVertices) {
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    if (newVertices.size() < vertices.size()) {
        glBufferSubData(GL_ARRAY_BUFFER, 0, newVertices.size() * sizeof(Vertex), &(newVertices[0]));
    } else {
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &(vertices[0]), GL_DYNAMIC_DRAW);
    }

    vertices = newVertices;
    glBindVertexArray(0);
}

}