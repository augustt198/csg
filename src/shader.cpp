#include "shader.h"

namespace csg {

ShaderProgram::ShaderProgram(const char *vertexShaderFile, const char *fragmentShaderFile) : successful(true) {
    GLuint vertexShader = makeShader(vertexShaderFile, GL_VERTEX_SHADER);
    GLuint fragmentShader = makeShader(fragmentShaderFile, GL_FRAGMENT_SHADER);

    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    glLinkProgram(program);
    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        this->successful = false;
        char log[1024] = {0};
        glGetProgramInfoLog(program, 1024, NULL, log);
        std::cout << "ERROR: failed to link program. Error log:" << std::endl;
        std::cout << log << std::endl;
    }

    // don't need to keep shaders anymore
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

GLuint ShaderProgram::makeShader(const char *filename, GLenum type) {
    std::ifstream fs(filename);
    std::stringstream ss;
    ss << fs.rdbuf();
    std::string source = ss.str();
    const char *source_cstr = source.c_str();

    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source_cstr, NULL);
    glCompileShader(shader);

    std::printf("Create shader %s\n", filename);

    // check compilation status
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        this->successful = false;
        std::string typeName = "";
        if (type == GL_VERTEX_SHADER)
            typeName = "vertex";
        else if (type == GL_FRAGMENT_SHADER)
            typeName = "fragment";

        std::cout << "ERROR: Failed to compile " << typeName << " shader. Error log:" << std::endl;

        char log[1024] = {0};
        glGetShaderInfoLog(shader, 1024, NULL, log);
        std::cout << log << std::endl;
    }

    return shader;
}

GLint ShaderProgram::uniformLocation(const char *name) {
    return glGetUniformLocation(program, name);
}

void ShaderProgram::setMat4(const char *name, glm::mat4 m) {
    glUniformMatrix4fv(uniformLocation(name), 1, GL_FALSE, &(m[0][0]));
}

void ShaderProgram::setVec3(const char *name, glm::vec3 v) {
    glUniform3fv(uniformLocation(name), 1, &(v[0]));
}

};