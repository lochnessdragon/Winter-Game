#pragma once

// System includes
#include <memory>

// Engine dependencies includes
#include <glm/glm.hpp>

// Engine includes
#include "renderer2d.h"

class Actor {
protected:
	glm::mat4 modelMat = glm::identity<glm::mat4>();
public:
	glm::vec2 position;
	float rotation;
	glm::vec2 scale;

	Actor(glm::vec2 position = { 0.0f, 0.0f }, float rotation = 0.0f, glm::vec2 scale = { 1.0f, 1.0f });

	void recalculateModelMat() {
		modelMat = glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f))
			* glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 0.0f, 1.0f))
			* glm::scale(glm::mat4(1.0f), glm::vec3(scale, 1.0f));
	}

	glm::mat4 getModelMat() { return modelMat; };

	/*virtual void update(double deltaTime) = 0;
	virtual void render(std::shared_ptr<Renderer2D> renderer) = 0;*/
};