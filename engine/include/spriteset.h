#pragma once

#include <memory>
#include <texture.h>

class Spriteset {
private:
	int tiles_per_row;
	int tiles_per_col;
public:
	std::shared_ptr<Texture> texture;
	int tileSize;

	Spriteset(std::shared_ptr<Texture> texture, int tileSize);

	glm::vec4 getUVCoords(int tileId);
	uint32_t getMaxId() { return tiles_per_row * tiles_per_col; };
};