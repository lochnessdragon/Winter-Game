// version directive handled by shader class
precision highp float;

layout (location = 0) in vec2 vPos;
layout (location = 1) in vec2 vUv;

//uniform mat4 modelMat;
//uniform mat4 viewMat;
//uniform mat4 projMat;

out vec2 uv;

void main() {
	uv = vUv;

	gl_Position = vec4(vPos, -1.0, 1.0);
}