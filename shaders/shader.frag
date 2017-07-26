#version 330 core

out vec4 FragColor;

in vec3 pos;
in vec3 normal;

void main() {
    vec3 lightPos = vec3(5.0, 5.0, 5.0);
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - pos);
    float diffuse = (dot(norm, lightDir) + 1.0)/2.0;

    FragColor = vec4(diffuse * abs(norm), 1.0);
    //FragColor = vec4(abs(norm), 1.0);
}
