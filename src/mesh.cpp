#include "mesh.h"

#include <cmath>
#include <cstdio>

namespace csg {

Mesh::Mesh() {
    currentVertices = NULL;
    newVertices = NULL;
    needsUpdate = false;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &(vertices[0]), GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    // the vertex positions - no offset b/c it's first in struct
    glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), (void*) 0);
    glEnableVertexAttribArray(1);
    // the normal vectors - offset by the vertex positions (Vec3)
    glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(Vertex), (void*) sizeof(Vec3));
    glBindVertexArray(0);
}

void Mesh::render() {
    if (currentVertices) {
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, currentVertices->size());
        glBindVertexArray(0);
    }
}

void Mesh::tryUpdate() {
    if (needsUpdate) {
        needsUpdate = false;

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        void *dataPtr = &(newVertices->front());
        glBufferData(GL_ARRAY_BUFFER, newVertices->size()*sizeof(Vertex), dataPtr, GL_DYNAMIC_DRAW);

        if (currentVertices) {
            delete currentVertices;
        }

        currentVertices = newVertices;
    }
}

void Mesh::updateVertices(std::vector<Vertex> *newVertices) {
    this->newVertices = newVertices;
    needsUpdate = true;
}

void Mesh::writeSTL(std::ostream& out, const char *solidName, bool ascii) {
    if (!currentVertices) return;
    std::vector<Vertex> vertices = *currentVertices;
    out << "solid " << solidName << std::endl;
    for (int i = 0; i < vertices.size(); i += 3) {
        // get face normal from vertex normals
        Vec3 n1 = vertices[i+0].normal;
        Vec3 n2 = vertices[i+1].normal;
        Vec3 n3 = vertices[i+2].normal;
        float avgX = (n1.x+n2.x+n3.x)/3.0,
              avgY = (n1.y+n2.y+n3.y)/3.0,
              avgZ = (n1.z+n2.z+n3.z)/3.0;
        // normalize avg
        float dist = std::sqrt(avgX*avgX + avgY*avgY + avgZ*avgZ);
        
        out << "facet normal " << avgX/dist << " " << avgY/dist << " " << avgZ/dist << std::endl;
        out << "    outer loop" << std::endl;
        for (int j = 0; j < 3; j++) {
            Vec3 v = vertices[i+j].position;
            out << "        vertex " << v.x << " " << v.y << " " << v.z << std::endl;
        }
        out << "    endloop" << std::endl;
        out << "endfacet" << std::endl;
    }
    out << "endsolid " << solidName << std::endl;
}

}