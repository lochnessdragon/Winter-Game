// version directive handled by shader class
precision highp float;

uniform sampler2D spriteTex;

in vec2 uv;

out vec4 FragColor;

void main() {
	FragColor = texture(spriteTex, uv);
}