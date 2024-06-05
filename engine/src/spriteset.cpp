#include <spriteset.h>

#include <cmath>

#include <log.h>
#include <glm/gtx/string_cast.hpp>

Spriteset::Spriteset(std::shared_ptr<Texture> texture, int tileSize) : texture(texture), tileSize(tileSize) {
	glm::ivec2 texSize = texture->getSize();
	tiles_per_row = texSize.x / tileSize;
	tiles_per_col = texSize.y / tileSize;
	Log::getRendererLog()->info("Loaded {} x {} tileset.", tiles_per_row, tiles_per_col);
}

glm::vec4 Spriteset::getUVCoords(int tileId) {
	glm::ivec2 texSize = texture->getSize();
	glm::vec4 uvs;

	uvs.x = (float)  (tileId % tiles_per_row) / (float)tiles_per_row;
	// define it backwards, because the top coord of the y is technically w
	uvs.w = 1.0f - (floor((float) tileId / (float) tiles_per_row) / (float) tiles_per_col);
	uvs.y = uvs.w - ((float)tileSize / (float)texSize.y);
	uvs.z = ((float) tileSize / (float) texSize.x) + uvs.x;

	return uvs;
}