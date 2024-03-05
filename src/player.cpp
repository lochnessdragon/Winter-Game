#include "player.h"

#include <imgui.h>
#include <scene/transform.h>

#include <scene/colliders.h>
#define __gl_h_
#include <tilemap.h>

#include <log.h>
#include <glm/gtx/string_cast.hpp>

#include <utils.h>

#include "room_manager.h"
#include "colors.h"
#include "consts.h"

Player::Player(float speed) : speed(speed), showColliders(false), colliderOffset({ 0, -5.0f }), colliderSize({ 0.75f, 0.35f }), facing(Direction::None), Actor({ 50.0f, 50.0f }, 0.0f, { 8.0f, 8.0f }) {
	texture = std::make_shared<Texture>("res/textures/player.png");
}

void Player::update(double deltaTime, std::shared_ptr<Tilemap> tilemap) {
	// apply full input
	glm::vec2 input = Input::get()->getInputAxis("walk");
	glm::vec2 displacement = input * speed * (float)deltaTime;

	// TODO: Implement physics stepping

	glm::vec2 test_player_pos = position;
	
	// check the tiles in the x plane
	// apply the move in the x direction
	test_player_pos.x += displacement.x;

	// check around for overlaps.
	// get the position of the player in tilemap coords
	int x_col = (test_player_pos.x + colliderOffset.x + copysign(colliderSize.x * scale.x, displacement.x)) / tilemap->getSpriteset()->tileSize;

	int bottom = (test_player_pos.y + colliderOffset.y - colliderSize.y * scale.y) / tilemap->getSpriteset()->tileSize;
	int top = (test_player_pos.y + colliderOffset.y + colliderSize.y * scale.y) / tilemap->getSpriteset()->tileSize;

	// collapse to bounds.
	if (x_col < 0) x_col = 0;
	if (x_col >= tilemap->width) x_col = tilemap->width - 1;
	if (bottom < 0) bottom = 0;
	if (top >= tilemap->height) top = tilemap->height - 1;

	// check the tiles around the player's position (making sure to account for size differences)
	bool collides = false;
	for (int y = bottom; y <= top; y++) {
		if (tilemap->isImpassable(glm::ivec2(x_col, y))) {
			collides = true;
			break;
		}
	}

	if (collides) {
		// halt movement, snapping to edge.
		int edge_x = (x_col + (displacement.x < 0 ? 1 : 0)) * tilemap->getSpriteset()->tileSize; // the x position of the collision edge
		float colliderRadius = colliderSize.x * scale.x; // not technically radius in the purely mathmatical sense, but represents the distance between the center of the collider and its edge (top or bottom)
		// snap the collider position to the edge.
		// 0.1f is to prevent the player from technically being right on the wall boundary, to leave some space between the player and the wall.
		float collider_x = edge_x - ((colliderRadius + 0.1f) * utils::sign(displacement.x));
		// convert collider position to player position
		position.x = collider_x - colliderOffset.x;
		displacement.x = 0;
	}

	// check the tiles on the y-plane
	// get the position of the player in tilemap coords
	test_player_pos = position;
	test_player_pos.y += displacement.y;

	int y_row = (test_player_pos.y + colliderOffset.y + copysign(colliderSize.y * scale.y, displacement.y)) / tilemap->getSpriteset()->tileSize;
	int left = (test_player_pos.x + colliderOffset.x - colliderSize.x * scale.x) / tilemap->getSpriteset()->tileSize;
	int right = (test_player_pos.x + colliderOffset.x + colliderSize.x * scale.x) / tilemap->getSpriteset()->tileSize;

	// collapse to bounds
	if (y_row < 0) y_row = 0;
	if (y_row >= tilemap->height) y_row = tilemap->height - 1;
	if (left < 0) left = 0;
	if (right >= tilemap->width) right = tilemap->width - 1;

	// check the tiles around the player's position (accounting for size difference)
	collides = false;
	for (int x = left; x <= right; x++) {
		if (tilemap->isImpassable(glm::ivec2(x, y_row))) {
			collides = true;
			break;
		}
	}
	
	if (collides) {
		// halt movement, snapping to edge
		int edge_y = (y_row + (displacement.y < 0 ? 1 : 0)) * tilemap->getSpriteset()->tileSize; // the y position of the collision edge
		float colliderRadius = colliderSize.y * scale.y; // not technically radius in the purely mathmatical sense, but represents the distance between the center of the collider and its edge (top or bottom)
		// snap the collider position to the edge.
		// 0.1f is to prevent the player from technically being right on the wall boundary, to leave some space between the player and the wall.
		float collider_y = edge_y - ((colliderRadius + 0.1f) * utils::sign(displacement.y));
		// convert collider position to player position
		position.y = collider_y - colliderOffset.y;
		displacement.y = 0;
	}

	// apply the displacement
	position += displacement;

	// load new room if within a certain distance from the side of the screen
	if (position.x < 0.0f) {
		// load west
		position = RoomManager::get()->next(Direction::Left, position);
	} else if (position.x > SCREEN_WIDTH) {
		// load east
		position = RoomManager::get()->next(Direction::Right, position);
	} else if (position.y < 0.0f) {
		// load south
		position = RoomManager::get()->next(Direction::Down, position);
	} else if (position.y > SCREEN_HEIGHT) {
		// load north
		position = RoomManager::get()->next(Direction::Up, position);
	}

	// we've probably changed the player's position this frame, so update that!
	recalculateModelMat();
}

void Player::render(std::shared_ptr<Renderer2D> renderer) {
	renderer->renderQuad(modelMat, texture);

	if (showColliders) {
		renderer->setLineWidth(1.0f);
		renderer->drawRect(position + colliderOffset, colliderSize * scale * 2.0f, Colors::Red);
	}
}

void Player::imgui() {
	ImGui::Begin("Player Information");
	ImGui::Text("Pos: <%f, %f>", position.x, position.y);
	ImGui::SliderFloat("Player speed", &speed, 0, 200);
	ImGui::Checkbox("Show colliders?", &showColliders);
	ImGui::SliderFloat2("Collider Offset", (float*) &colliderOffset, -16.0f, 16.0f);
	ImGui::SliderFloat2("Collider Size", (float*) &colliderSize, 0.05f, 2.0f);
	ImGui::End();
}