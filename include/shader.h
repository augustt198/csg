#pragma once

#include <GL/gl3w.h>

#include <glm/glm.hpp>

#include <fstream>
#include <sstream>
#include <iostream>

namespace csg {

class ShaderProgram {

    public:
    GLuint program;
    bool successful;

    ShaderProgram(const char *vertexShaderFile, const char *fragmentShaderFile);

    void setMat4(const char *name, glm::mat4 m);
    void setVec3(const char *name, glm::vec3 v);

    private:
    GLuint makeShader(const char *filename, GLenum type);
    GLint uniformLocation(const char *name);

};

}
