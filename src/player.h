#pragma once

#include <entt/entt.hpp>
#include <input.h>

#include <renderer2d.h>

enum class Direction {
	Up,
	Down,
	Left,
	Right
};

struct PlayerComponent {
	float speed = 5.0f;
	bool show_colliders = false;
	Direction facing = Direction::Up;
};

void player_move(double deltaTime, entt::registry& scene, entt::entity player, entt::entity tilemap);
void player_render(std::shared_ptr<Renderer2D> renderer, entt::registry& scene, entt::entity player);
void player_debug(entt::registry& scene, entt::entity player);