
layout (location = 0) in vec3 pos;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;

void main() {
    gl_Position = projMat * viewMat * modelMat * vec4(pos, 1); // projMat * viewMat * modelMat * 
}