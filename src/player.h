#pragma once

#include <entt/entt.hpp>
#include <input.h>

struct PlayerComponent {
	float speed;
	// facing
};

void player_move(double deltaTime, entt::registry& scene, entt::entity player, entt::entity tilemap);
void player_debug(entt::registry& scene, entt::entity player);