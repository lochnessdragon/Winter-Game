#include "renderer2d.h"

#include "platform.h"
#ifndef PLATFORM_WEB
#include <glad/glad.h>
#else
#include <GLES3/gl3.h>
#endif
#include <GLFW/glfw3.h>

Renderer2D::Renderer2D()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
}

void Renderer2D::clear() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer2D::setClearColor(float red, float green, float blue, float alpha) {
	glClearColor(red, green, blue, alpha);
}

void Renderer2D::setClearColor(glm::vec4 color) {
	glClearColor(color.r, color.g, color.b, color.a);
}

void Renderer2D::startFrame() {
	this->clear();
}

