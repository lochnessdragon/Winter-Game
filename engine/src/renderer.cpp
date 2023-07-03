#include "renderer.h"

#include "platform.h"
#ifndef PLATFORM_WEB
#include <glad/glad.h>
#else
#include <GLES3/gl3.h>
#endif
#include <GLFW/glfw3.h>

Renderer::Renderer() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}

void Renderer::clear() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::setClearColor(float red, float green, float blue, float alpha) {
	glClearColor(red, green, blue, alpha);
}

void Renderer::setClearColor(glm::vec4 color) {
	glClearColor(color.r, color.g, color.b, color.a);
}

void Renderer::startFrame() {
	this->clear();
}

void Renderer::render(std::shared_ptr<Camera> camera, std::shared_ptr<Object> object, std::shared_ptr<Mesh> mesh, std::shared_ptr<Shader> shader, std::shared_ptr<Texture> texture) {
	shader->use();
	shader->loadUniform("modelMat", object->getModelMat());
	shader->loadUniform("viewMat", camera->getViewMat());
	shader->loadUniform("projMat", camera->getProjMat());
	shader->loadUniform("texture0", 0);
	
	glActiveTexture(GL_TEXTURE0);
	texture->bind();
	mesh->bind();
	glDrawElements(GL_TRIANGLES, mesh->getIndexCount(), GL_UNSIGNED_INT, 0);
	mesh->unbind();
}
