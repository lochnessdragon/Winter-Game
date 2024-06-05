#include <tilemap.h>

#include <stdexcept>
#include <iostream>
#include <fstream>

#include <log.h>
#include <io.h>

Tilemap::Tilemap(int width, int height, std::shared_ptr<Spriteset> tileset) : width(width), height(height), tileset(tileset), Actor({0.0f, 0.0f}, 0.0f, glm::vec2(1.0f)) {
	grid = new int[width * height];

	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			set({ x, y }, 0);
		}
	}
}

Tilemap::Tilemap(const std::string& filename, std::shared_ptr<Spriteset> tileset) : tileset(tileset), grid(nullptr), Actor({ 0.0f, 0.0f }, 0.0f, glm::vec2(1.0f)) {
	load(filename);
}

Tilemap::Tilemap(const char* filename, std::shared_ptr<Spriteset> tileset) : Tilemap(std::string(filename), tileset) {}

Tilemap::~Tilemap() {
	delete grid;
}

void Tilemap::set(glm::ivec2 pos, int id) {
	if (id > (int) tileset->getMaxId() || id < -1)
		throw std::runtime_error("Id: " + id + std::string(" is out of bounds for what the tileset can support."));

	if (pos.x < 0 || pos.x >= width || pos.y < 0 || pos.y >= height)
		throw std::runtime_error("Tilemap position out of bounds.");
	grid[(pos.y * width) + pos.x] = id;
}

int Tilemap::get(glm::ivec2 pos) {
	if (pos.x < 0 || pos.x >= width || pos.y < 0 || pos.y >= height)
		throw std::runtime_error("Tilemap index out of bounds.");
	return grid[(pos.y * width) + pos.x];
}

void Tilemap::save(const std::string& filename) {
	std::ofstream file(filename, std::ios::binary);

	file.write("MAP", 3);
	file.write((char*)  &width, sizeof(int));
	file.write((char*) &height, sizeof(int));

	for (int i = 0; i < (width * height); i++) {
		file.write((char*) &grid[i], sizeof(int));
	}

	Log::getGameLog()->info("Tilemap saved as {}", filename);
}

void Tilemap::load(const std::string& filename) {
	if (grid)
		delete grid;
	std::ifstream file(filename, std::ios::binary);
	std::streamsize fileLength = io::getFileSize(file);
	if (file.is_open()) {
		char code[3] = { 0 };
		file.read(code, 3);
		// don't allow reading just any binary file, if it doesn't have the magic word, it ain't ours.
		if (strncmp(code, "MAP", 3) == 0) {
			// this is a tilemap file
			file.read((char*)&width, sizeof(int));
			file.read((char*)&height, sizeof(int));

			// error check width and height
			if (width < 1 || height < 1 || width > 64 || height > 64) { // upper bounds is kinda dumb, but will help me catch errors
				Log::getGameLog()->critical("Tilemap: {} is probably corrupt, width and height are extreme : ({}, {})", filename, width, height);
				throw std::runtime_error("Corrupt tilemap");
			}

			if ((width * height) > fileLength) {
				Log::getGameLog()->critical("Tilemap has been truncated, should be at least {} bytes long. (real = {})", width * height, fileLength);
			}

			Log::getGameLog()->trace("Loading {} x {} tilemap from file: {}", width, height, filename);
			grid = new int[width * height];
			for (int i = 0; i < (width * height); i++) {
				int next = 0;
				file.read((char*)&next, sizeof(int));
				grid[i] = next;
			}

			if (!file) {
				Log::getGameLog()->critical("Error: not enough data in file: {} to recreate tilemap.", filename);
				throw std::runtime_error("Overflow tilemap file bounds");
			}
		}
		else {
			Log::getGameLog()->critical("Error loading tilemap: {}", filename);
			throw std::runtime_error(filename + " is not a tilemap file.");
		}
	}
}

void Tilemap::render(std::shared_ptr<Renderer2D> renderer) {
	int baseX = tileset->tileSize / 2;
	int baseY = tileset->tileSize / 2;

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			int tileId = get({ x, y });
			
			// skip empty
			if (tileId >= 0) {
				// this is an absolute block of a unit. Basically converts the tile position to an onscreen position in coords.
				// this is very specific for an orthographic camera
				// also, it doesn't support transforming the tilemap, which is defininitely a TODO!
				glm::mat4 modelMat = glm::translate(glm::mat4(1), { 
					((float)x * tileset->tileSize) + (float) baseX, 
					((float)y * tileset->tileSize) + (float) baseY, 
					0.0f }) * glm::scale(glm::mat4(1), glm::vec3(tileset->tileSize / 2));

				// finally, add the quad to the buffer
				renderer->renderQuad(modelMat, tileset->texture, tileset->getUVCoords(tileId));
			}
		}
	}
}