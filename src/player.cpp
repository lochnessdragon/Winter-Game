#include "player.h"

#include <imgui.h>
#include <scene/transform.h>

#include <scene/colliders.h>
#define __gl_h_
#include <tilemap.h>

#include <log.h>
#include <glm/gtx/string_cast.hpp>

void player_move(double deltaTime, entt::registry& scene, entt::entity player, entt::entity tilemap) {
	auto& transform = scene.get<Transform>(player);
	auto& player_comp = scene.get<PlayerComponent>(player);
	auto& tilemap_comp = scene.get<Tilemap>(tilemap);
	auto& tilemap_collider = scene.get<TilemapCollider2D>(tilemap);

	glm::vec2 input = Input::get()->getInputAxis("walk");
	glm::vec2 displacement = input * player_comp.speed * (float)deltaTime;

	// check the tiles around the player's position

	transform.position += glm::vec3(displacement, 0.0f);

	transform.dirty = true;
}

void player_debug(entt::registry& scene, entt::entity player) {
	auto& player_comp = scene.get<PlayerComponent>(player);

	ImGui::Begin("Player settings");
	ImGui::SliderFloat("Player speed", &player_comp.speed, 0, 200);
	ImGui::End();
}