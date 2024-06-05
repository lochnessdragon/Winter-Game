// version directive handled by shader class 
//! #version 330 core 
precision highp float;

in vec4 color;

out vec4 FragColor;

void main() {
	FragColor = color;
}