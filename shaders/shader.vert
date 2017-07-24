#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 pos;
out vec3 normal;

void main() {
    pos = aPos;
    normal = aNormal;

    gl_Position = vec4(aPos, 1.0);
}
