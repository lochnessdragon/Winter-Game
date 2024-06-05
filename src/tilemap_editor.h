#pragma once

#include <string>

#include <tilemap.h>
#include <window.h>

class TilemapEditor {
private:
	int currentBrushId;

	std::shared_ptr<Tilemap> tilemap;
	std::string filename;
	std::shared_ptr<Window> window;

	bool active = false;
public:
	TilemapEditor(std::string filename, std::shared_ptr<Window> win, std::shared_ptr<Tilemap> tilemap);

	void setTilemap(std::shared_ptr<Tilemap> tilemap) { this->tilemap = tilemap; };

	void update();
	void imgui();
};