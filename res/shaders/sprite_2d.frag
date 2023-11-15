// version directive handled by shader class
precision highp float;

uniform sampler2D spriteTex;

in vec2 uv;
in vec4 color;

out vec4 FragColor;

void main() {
	FragColor = color * texture(spriteTex, uv);
}