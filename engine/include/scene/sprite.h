#pragma once

#include <memory>
#include <glm/glm.hpp>
#include "texture.h"

struct SpriteComponent {
	// need texture, color
	glm::vec4 color = { 1.0, 1.0, 1.0, 1.0 };
	std::shared_ptr<Texture> texture;
};