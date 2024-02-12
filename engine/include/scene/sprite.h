#pragma once

#include <memory>
#include <glm/glm.hpp>
#include "texture.h"
#include "spriteset.h"

struct SpriteComponent {
	// need texture, color
	glm::vec4 color = { 1.0, 1.0, 1.0, 1.0 };
	glm::vec4 uvs = { 0.0, 0.0, 1.0, 1.0 };
	std::shared_ptr<Texture> texture;
};

struct SpriteAtlas {
	std::shared_ptr<Spriteset> spritesheet;
	uint32_t id;
	glm::vec4 color = { 1.0, 1.0, 1.0, 1.0 };
};