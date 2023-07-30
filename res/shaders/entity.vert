// version directive handled by shader class
precision highp float;

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 vUv;
layout (location = 2) in vec3 vNormal;

// consider combining model view proj into MVP mat
uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;

uniform mat3 normalMat; // inverse transpose of the view * model matrices

uniform vec3 lightPos; // where the light is, in world space

out vec2 uv;
out vec3 normal;
out vec3 fragPosViewSpace;
out vec3 lightPosViewSpace;

void main() {
    uv = vUv;
    normal = normalMat * vNormal; // look at normal matrices for non-uniform scaling operations
    fragPosViewSpace = vec3(viewMat * modelMat * vec4(pos, 1.0)); // required for calculating the lighting dot product
    lightPosViewSpace = vec3(viewMat * vec4(lightPos, 1.0)); // could be calculated once on the cpu as sent as a uniform

    // finally, calculate the gl_Position
    gl_Position = projMat * viewMat * modelMat * vec4(pos, 1); // projMat * viewMat * modelMat * 
}
