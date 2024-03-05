#pragma once

#include <string>

#include "GLAPI.h"

#include <glm/glm.hpp>

enum class TextureScaling {
	Nearest,
	Linear
};

struct TextureSpecification {
	int width = 0;
	int height = 0;
	GLenum format = GL_RGBA; // per-pixel format
	TextureScaling scaling = TextureScaling::Linear;
};

class Texture {
private:
	GLuint handle;
	glm::ivec2 size;

public:
	Texture(TextureSpecification spec, const unsigned char* data);
	Texture(std::string filename, TextureScaling scaling = TextureScaling::Nearest);
	~Texture() { glDeleteTextures(1, &this->handle); };

	const glm::ivec2& getSize() { return this->size; };
	int width() { return size.x; };
	int height() { return size.y; };
	void bind() { glBindTexture(GL_TEXTURE_2D, this->handle); };

	bool isTexture() { return glIsTexture(handle); };

	GLuint getHandle() { return handle; };

	/**
	* getUVCoordsFromPixels - returns the uv coordinates: [0, 1], [0, 1] of a region of an image based on given pixel coordinates: [0, width), [0, height).
	*  @param pixelCoords is based on the top left of the texture
	*  @returns uv coords based on the bottom left of the texture
	*/
	glm::vec4 getUVCoordsFromPixels(glm::vec4 pixelCoords);

protected:
	friend bool operator<(const Texture& lhs, const Texture& rhs);
	friend bool operator==(const Texture& lhs, const Texture& rhs);
};