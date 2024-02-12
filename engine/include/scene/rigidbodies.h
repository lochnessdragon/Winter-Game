#pragma once

// Engine - depedency files
#include <glm/glm.hpp>

// Engine files

struct Rigidbody2D {
	bool enabled; // should we use this rigidbody?
	glm::vec2 velocity; // speed of the object
	glm::vec2 acceleration; // acceleration from last frame
	float mass; // kg
};