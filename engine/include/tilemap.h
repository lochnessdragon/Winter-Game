#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <spriteset.h>

struct Tilemap {
private:
	int* grid; // technically a 2d array

	std::shared_ptr<Spriteset> tileset;
public:
	int width;
	int height;

	Tilemap(int width, int height, std::shared_ptr<Spriteset> tileset);
	Tilemap(const std::string& filename, std::shared_ptr<Spriteset> tileset);
	Tilemap(const char* filename, std::shared_ptr<Spriteset> tileset);
	~Tilemap();

	void set(glm::ivec2 pos, int id);
	int get(glm::ivec2 pos);

	void save(const std::string& filename);
	void load(const std::string& filename);

	bool isInBounds(glm::ivec2 pos) { return (pos.x >= 0 && pos.y >= 0 && pos.x < width && pos.y < height) ? true : false; }

	std::shared_ptr<Spriteset> getSpriteset() { return tileset; };
};