#pragma once

#include <string>

#include <tilemap.h>
#include <entt/entt.hpp>
#include <window.h>

class TilemapEditor {
private:
	int currentBrushId;

	entt::entity tilemapEntity;
	std::string filename;
	std::shared_ptr<Window> window;

	bool active = false;
public:
	TilemapEditor(std::string filename, std::shared_ptr<Window> win);

	void setTilemap(entt::entity tilemap) { tilemapEntity = tilemap; };

	void update(entt::registry& scene);
	void gui(entt::registry& scene);
};