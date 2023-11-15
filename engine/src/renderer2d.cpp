#include "renderer2d.h"

#include "platform.h"
#ifndef PLATFORM_WEB
#include <glad/glad.h>
#else
#include <GLES3/gl3.h>
#endif
#include <GLFW/glfw3.h>

#include "log.h"

/**
* Triangle Idxs
* [0] = Top-left
* [1] = Top-right
* [2] = Bottom-left
* [3] = Bottom-right
*/

// don't know if I love the white texture's initialization. It's concise, but not readable
Renderer2D::Renderer2D() : whiteTex(createWhiteTexture()), spriteShader("res/shaders/sprite_2d.vert", "res/shaders/sprite_2d.frag"), dynMesh(createDynMesh()), quadIdx(0) {
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
}

std::shared_ptr<Texture> Renderer2D::createWhiteTexture() {
	TextureSpecification spec;
	spec.height = 1;
	spec.width = 1;
	unsigned char data[4] = {255, 255, 255, 255};
	return std::make_shared<Texture>(spec, data);
}

std::shared_ptr<Mesh> Renderer2D::createDynMesh() {
	// generate indices [0, 1, 2, 0, 2, 3] = 1 triangle
	unsigned int indices[MAX_INDICES] = { 0 };
	for (unsigned int i = 0; i < (MAX_INDICES / 6); i++) {
		unsigned int real_idx = i * 6;
		unsigned int base_index = i * 4;
		indices[real_idx] = base_index;
		indices[real_idx + 1] = base_index + 2;
		indices[real_idx + 2] = base_index + 1;
		indices[real_idx + 3] = base_index + 1;
		indices[real_idx + 4] = base_index + 2;
		indices[real_idx + 5] = base_index + 3;
	}

	auto mesh = std::make_shared<Mesh>(MAX_INDICES, indices, MAX_VERTICES * 3, nullptr, MAX_VERTICES * 2, nullptr, MeshUsage::Dynamic); // I am assured OpenGL doesn't copy when a nullptr is passed to glBufferData
	mesh->attachBuffer(GL_FLOAT, 4, MAX_VERTICES * 4, nullptr);
	return mesh;
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

void Renderer2D::renderSprite(std::shared_ptr<Camera> camera, Transform transform) {
	renderSprite(camera, transform, whiteTex);
}

void Renderer2D::renderSprite(std::shared_ptr<Camera> camera, Transform transform, std::shared_ptr<Texture> texture, glm::vec4 color) {
	if (quadIdx >= MAX_QUADS)
		flush();
	
	if (!texture) {
		texture = whiteTex;
	}

	// skip non-consistency checks if this is our first quad
	if (quadIdx > 0) {
		if (viewMat != camera->getViewMat() || projMat != camera->getProjMat() || currTex != texture) {
			flush();
		}
	}

	viewMat = camera->getViewMat();
	projMat = camera->getProjMat();
	currTex = texture;

	glm::mat4 modelMat = transform.getModelMat();
	uint32_t vertex_idx = quadIdx * 4;

	// quad vertices
	vertexData[vertex_idx] = modelMat * glm::vec4(-1.0, 1.0, 0.0, 1.0);
	vertexData[vertex_idx + 1] = modelMat * glm::vec4(1.0, 1.0, 0.0, 1.0);
	vertexData[vertex_idx + 2] = modelMat * glm::vec4(-1.0, -1.0, 0.0, 1.0);
	vertexData[vertex_idx + 3] = modelMat * glm::vec4(1.0, -1.0, 0.0, 1.0);

	uint32_t uv_idx = quadIdx * 4 * 2;

	// top-left
	uvData[uv_idx] = 0.0;
	uvData[uv_idx + 1] = 1.0;

	// top-right
	uvData[uv_idx + 2] = 1.0;
	uvData[uv_idx + 3] = 1.0;

	// bottom-left
	uvData[uv_idx + 4] = 0.0;
	uvData[uv_idx + 5] = 0.0;

	// bottom-right
	uvData[uv_idx + 6] = 1.0;
	uvData[uv_idx + 7] = 0.0;

	uint32_t color_idx = quadIdx * 4 * 4;
	for (uint32_t i = 0; i < 4; i++) {
		colorData[color_idx + (i * 4)] = color.x;
		colorData[color_idx + (i * 4) + 1] = color.y;
		colorData[color_idx + (i * 4) + 2] = color.z;
		colorData[color_idx + (i * 4) + 3] = color.w;
	}
	
	quadIdx++;
}

void Renderer2D::flush() {
	spriteShader.use();
	spriteShader.loadUniform("VPMat", projMat * viewMat);
	spriteShader.loadUniform("spriteTex", 0);

	glActiveTexture(GL_TEXTURE0);
	currTex->bind();

	dynMesh->bind();
	dynMesh->setBuffer(0, quadIdx * 4 * sizeof(glm::vec3), vertexData);
	dynMesh->setBuffer(1, quadIdx * 4 * 2 * sizeof(float), uvData);
	dynMesh->setBuffer(2, quadIdx * 4 * 4 * sizeof(float), colorData);
	glDrawElements(GL_TRIANGLES, quadIdx * 6, GL_UNSIGNED_INT, 0);
	dynMesh->unbind();

	quadIdx = 0;
	currTex.reset();
}

void Renderer2D::endFrame() {
	flush();
}

