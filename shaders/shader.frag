#version 330 core

out vec4 FragColor;

in vec3 pos;
in vec3 normal;

void main() {
    vec3 lightPos = vec3(5.0, 5.0, 5.0);
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - pos);
    float diffuse = dot(norm, lightDir);

    FragColor = vec4(diffuse * vec3(1.0), 1.0);
    //FragColor = vec4(abs(norm), 1.0);
}
