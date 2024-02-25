// version directive handeled by shader class
//! #version 330 core
precision highp float;

layout (location = 0) in vec2 vPos;
layout (location = 1) in vec4 vColor;

uniform mat4 VPMat;

out vec4 color;

void main() {
	gl_Position = VPMat * vec4(vPos, 1.0, 1.0);
	color = vColor;
}