#pragma once

#include <input.h>

#include <renderer2d.h>
#include <tilemap.h>

#include "direction.h"

class Player : public Actor {
private:
	std::shared_ptr<Texture> texture;
	glm::vec2 colliderOffset;
	glm::vec2 colliderSize;

	Direction facing;
public:
	float speed;
	bool showColliders = false;

	Player(float speed);

	void update(double deltaTime, std::shared_ptr<Tilemap> tilemap);
	void render(std::shared_ptr<Renderer2D> renderer);
	void imgui();
};