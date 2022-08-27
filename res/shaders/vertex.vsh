#version 330 core

layout (location = 0) in vec3 pos;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;

out vec3 vertColor;

void main() {
    gl_Position = projMat * viewMat * modelMat * vec4(pos, 1);
    vertColor = pos;
}