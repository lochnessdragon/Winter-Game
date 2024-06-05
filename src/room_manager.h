#pragma once

#include <memory>

#include <tilemap.h>

#include "direction.h"

struct RoomEntry {
	glm::vec2 start_pos = { 0.0f, 0.0f };
	std::string filename = "";
	std::string up = "";
	std::string down = "";
	std::string left = "";
	std::string right = "";
};

class RoomManager {
private:
	std::shared_ptr<Tilemap> tilemap;

	std::unordered_map<std::string, RoomEntry> rooms;
	std::string currentRoom;

	static RoomManager* singleton;
public:
	RoomManager();
	RoomManager(std::shared_ptr<Tilemap> tilemap);

	void setTilemap(std::shared_ptr<Tilemap> tilemap) { this->tilemap = tilemap; };

	// returns true if there is a room in the specified direction
	bool hasNext(Direction going);
	// loads the next room in a direction and returns the position that the player should start at.
	glm::vec2 next(Direction going, glm::vec2 playerPos);
	// loads the next room (any room) and returns the position that the player should go to.
	glm::vec2 enter(std::string id);

	void configRoom(std::string id, RoomEntry entry) { rooms[id] = entry; };
	RoomEntry& getEntry(std::string id) { return rooms.at(id); };

	static RoomManager* get() { if (!singleton) { throw std::runtime_error("Singleton doesn't exist for room manager class"); }; return singleton; };
};