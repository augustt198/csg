#version 330 core

out vec4 FragColor;

in vec3 pos;
in vec3 normal;

uniform vec3 cameraPos;

vec3 hsv2rgb(vec3 c) {
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

const float PI = 3.1415926535897932384626433832795;

void main() {
    vec3 lightPos = vec3(5.0, 5.0, 5.0);
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - pos);
    float diffuse = dot(norm, lightDir);


    vec3 vDir = normalize(cameraPos - pos);
    float cosAngle = dot(vDir, norm);
    float hue = acos(cosAngle) / PI;
    vec3 color = hsv2rgb(vec3(hue, 1.0, 1.0));

    FragColor = vec4(diffuse * vec3(1.0), 1.0);
    //FragColor = vec4(abs(norm), 1.0);
    //FragColor = vec4(color, 1.0);
}
