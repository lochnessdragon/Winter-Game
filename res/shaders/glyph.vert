// version directive handled by shader class
precision highp float;

layout (location = 0) in vec2 vPos;
layout (location = 1) in vec2 vUv;

uniform mat4 projMat;

out vec2 uv;

void main() {
	uv = vUv; // and that's the easy one

	gl_Position = projMat * vec4(vPos, 0.0, 1.0);
}