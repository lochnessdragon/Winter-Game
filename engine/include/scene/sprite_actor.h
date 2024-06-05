#pragma once

#include <memory>
#include <glm/glm.hpp>
#include "texture.h"
#include "spriteset.h"
#include "actor.h"

class SpriteActor : public Actor {
public:
	// need texture, color
	glm::vec4 color = { 1.0, 1.0, 1.0, 1.0 };
	glm::vec4 uvs = { 0.0, 0.0, 1.0, 1.0 };
	std::shared_ptr<Texture> texture = nullptr;

	SpriteActor(std::shared_ptr<Texture> texture);
	SpriteActor(glm::vec2 position = { 0.0f, 0.0f }, float rotation = 0.0f, glm::vec2 scale = { 1.0f, 1.0f }, std::shared_ptr<Texture> texture = nullptr, glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f }, glm::vec4 uvs = { 0.0, 0.0, 1.0, 1.0 });

	void render(std::shared_ptr<Renderer2D> renderer);
};

struct SpriteAtlas {
	std::shared_ptr<Spriteset> spritesheet;
	uint32_t id;
	glm::vec4 color = { 1.0, 1.0, 1.0, 1.0 };
};