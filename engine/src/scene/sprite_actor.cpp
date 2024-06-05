#include "scene/sprite_actor.h"

SpriteActor::SpriteActor(std::shared_ptr<Texture> texture) : Actor({ 0.0f, 0.0f }, 0.0f, { texture->width() / 2.0f, texture->height() / 2.0f }), texture(texture) {}

SpriteActor::SpriteActor(glm::vec2 position, float rotation, glm::vec2 scale, std::shared_ptr<Texture> texture, glm::vec4 color, glm::vec4 uvs) : Actor(position, rotation, scale), texture(texture), color(color), uvs(uvs) {
	if (scale == glm::vec2(1.0f, 1.0f) && texture != nullptr) {
		scale = glm::vec2(texture->width() / 2.0f, texture->height() / 2.0f);
	}
}

void SpriteActor::render(std::shared_ptr<Renderer2D> renderer) {
	renderer->renderQuad(this->modelMat, texture, uvs, color);
}