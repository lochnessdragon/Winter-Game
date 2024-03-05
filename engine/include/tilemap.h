#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <spriteset.h>

#include <scene/actor.h>

class Tilemap : public Actor {
private:
	int* grid; // technically a 2d array

	std::shared_ptr<Spriteset> tileset;
public:
	int width;
	int height;
	std::vector<int> impassable;

	Tilemap(int width, int height, std::shared_ptr<Spriteset> tileset);
	Tilemap(const std::string& filename, std::shared_ptr<Spriteset> tileset);
	Tilemap(const char* filename, std::shared_ptr<Spriteset> tileset);
	~Tilemap();

	void set(glm::ivec2 pos, int id);
	int get(glm::ivec2 pos);

	void save(const std::string& filename);
	void load(const std::string& filename);

	void render(std::shared_ptr<Renderer2D> renderer);

	bool isInBounds(glm::ivec2 pos) { return (pos.x >= 0 && pos.y >= 0 && pos.x < width && pos.y < height) ? true : false; }
	bool isImpassable(glm::ivec2 pos) { return std::find(impassable.begin(), impassable.end(), get(pos)) != impassable.end(); }

	std::shared_ptr<Spriteset> getSpriteset() { return tileset; };
};