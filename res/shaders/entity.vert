// version directive handled by shader class
precision highp float;

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 vUv;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;

out vec2 uv;

void main() {
    uv = vUv;
    gl_Position = projMat * viewMat * modelMat * vec4(pos, 1); // projMat * viewMat * modelMat * 
}