#include "player.h"

#include <imgui.h>
#include <scene/transform.h>

#include <scene/colliders.h>
#define __gl_h_
#include <tilemap.h>

#include <log.h>
#include <glm/gtx/string_cast.hpp>

#include "colors.h"

void player_move(double deltaTime, entt::registry& scene, entt::entity player, entt::entity tilemap) {
	auto& transform = scene.get<Transform>(player);
	auto& player_comp = scene.get<PlayerComponent>(player);
	auto& player_collider = scene.get<RectangleCollider2D>(player);

	// tilemap components
	auto& tilemap_comp = scene.get<Tilemap>(tilemap);
	auto& tilemap_collider = scene.get<TilemapCollider2D>(tilemap);

	// apply full input
	glm::vec2 input = Input::get()->getInputAxis("walk");
	glm::vec2 displacement = input * player_comp.speed * (float)deltaTime;

	glm::vec2 player_pos = glm::vec2(transform.position);
	
	// apply the move in the x direction
	player_pos.x += displacement.x;

	// check around for overlaps.
	// get the position of the player in tilemap coords
	int left = (player_pos.x + player_collider.offset.x - player_collider.size.x * transform.scale.x) / tilemap_comp.getSpriteset()->tileSize;
	int right = (player_pos.x + player_collider.offset.x + player_collider.size.x * transform.scale.x) / tilemap_comp.getSpriteset()->tileSize;
	int bottom = (player_pos.y + player_collider.offset.y - player_collider.size.y * transform.scale.y) / tilemap_comp.getSpriteset()->tileSize;
	int top = (player_pos.y + player_collider.offset.y + player_collider.size.y * transform.scale.y) / tilemap_comp.getSpriteset()->tileSize;

	// collapse to bounds.
	if (left < 0) left = 0;
	if (right >= tilemap_comp.width) right = tilemap_comp.width - 1;
	if (bottom < 0) bottom = 0;
	if (top >= tilemap_comp.height) top = tilemap_comp.height - 1;

	// check the tiles around the player's position (making sure to account for size differences)
	bool collides = false;
	for (int i = left; i <= right; i++) {
		for (int j = bottom; j <= top; j++) {
			if (std::find(tilemap_collider.impassable.begin(), tilemap_collider.impassable.end(), tilemap_comp.get(glm::ivec2(i, j))) != tilemap_collider.impassable.end() && intersects()) {
				collides = true; 
				break;
			}
		}
	}

	if (collides) {
		// halt movement, clamping to edge.
	}

	// check the tiles on the x-plane

	// check the tiles on the y-plane
	
	// apply the displacement
	transform.position += glm::vec3(displacement, 0.0f);

	transform.dirty = true;
}

void player_render(std::shared_ptr<Renderer2D> renderer, entt::registry& scene, entt::entity player) {
	auto& transform = scene.get<Transform>(player);
	auto& player_comp = scene.get<PlayerComponent>(player);
	auto& rectangle_collider = scene.get<RectangleCollider2D>(player);

	if (player_comp.show_colliders) {
		renderer->setLineWidth(1.0f);
		renderer->drawRect(glm::vec2(transform.position) + rectangle_collider.offset, rectangle_collider.size * glm::vec2(transform.scale) * 2.0f, Colors::Red);
	}
}

void player_debug(entt::registry& scene, entt::entity player) {
	auto& player_comp = scene.get<PlayerComponent>(player);
	auto& rectangle_collider = scene.get<RectangleCollider2D>(player);

	ImGui::Begin("Player settings");
	ImGui::SliderFloat("Player speed", &player_comp.speed, 0, 200);
	ImGui::Checkbox("Show colliders?", &player_comp.show_colliders);
	ImGui::SliderFloat2("Collider Offset", (float*) &rectangle_collider.offset, -16.0f, 16.0f);
	ImGui::SliderFloat2("Collider Size", (float*) &rectangle_collider.size, 0.05f, 2.0f);
	ImGui::End();
}