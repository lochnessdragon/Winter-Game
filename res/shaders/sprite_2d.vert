// version directive handled by shader class
precision highp float;

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec2 vUv;
layout (location = 2) in vec4 vColor;
layout (location = 3) in float vTexIndex;

//uniform mat4 modelMat; the model matrix is premultiplied in batching code
// consider combining into VP matrix
uniform mat4 VPMat;

out vec2 uv;
out vec4 color;
out float texIndex;

void main() {
	uv = vUv;
	color = vColor;
	texIndex = vTexIndex;

	gl_Position = VPMat * vec4(vPos, 1.0);
}