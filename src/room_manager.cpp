#include "room_manager.h"

#include "consts.h"

#include "log.h"

RoomManager* RoomManager::singleton = nullptr;

RoomManager::RoomManager() {
	singleton = this;
}

RoomManager::RoomManager(std::shared_ptr<Tilemap> tilemap) : tilemap(tilemap), currentRoom("") {
	singleton = this;
}

bool RoomManager::hasNext(Direction going)
{
	RoomEntry entry = rooms.at(currentRoom);
	switch (going) {
	case Direction::Up:
		return !entry.up.empty();
	case Direction::Down:
		return !entry.down.empty();
	case Direction::Left:
		return !entry.left.empty();
	case Direction::Right:
		return !entry.right.empty();
	default:
		return false;
	}
}

glm::vec2 RoomManager::next(Direction going, glm::vec2 playerPos)
{
	RoomEntry entry = rooms.at(currentRoom);

	std::string next_room_id;

	glm::vec2 nextPlayerPos = playerPos;

	switch (going) {
	case Direction::Up:
		next_room_id = entry.up;
		nextPlayerPos.y = 0;
		break;
	case Direction::Down:
		next_room_id = entry.down;
		nextPlayerPos.y = SCREEN_HEIGHT;
		break;
	case Direction::Left:
		next_room_id = entry.left;
		nextPlayerPos.x = SCREEN_WIDTH;
		break;
	case Direction::Right:
		next_room_id = entry.right;
		nextPlayerPos.x = 0.0f;
		break;
	}

	if (next_room_id == "") {
		Log::getGameLog()->warn("No room in direction: {}!", going);
		return playerPos;
	}

	RoomEntry next = rooms.at(next_room_id);

	tilemap->load(next.filename);

	currentRoom = next_room_id;

	return nextPlayerPos;
}

glm::vec2 RoomManager::enter(std::string id)
{
	RoomEntry entry = rooms.at(id);

	tilemap->load(entry.filename);

	currentRoom = id;
	return entry.start_pos;
}
