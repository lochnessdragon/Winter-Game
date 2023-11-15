// version directive handled by shader class
precision highp float;

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec2 vUv;
layout (location = 2) in vec4 vColor;

//uniform mat4 modelMat;
// consider combining into VP matrix
uniform mat4 VPMat;

out vec2 uv;
out vec4 color;

void main() {
	uv = vUv;
	color = vColor;

	gl_Position = VPMat * vec4(vPos, 1.0);
}