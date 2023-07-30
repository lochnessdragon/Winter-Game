#pragma once

#include <string>

#include "GLAPI.h"

#include <glm/glm.hpp>

class Texture {
private:
	GLuint handle;
	glm::ivec2 size;

public:
	Texture(int width, int height, GLenum format, unsigned char* data);
	Texture(std::string filename);
	~Texture() { glDeleteTextures(1, &this->handle); };

	const glm::ivec2& getSize() { return this->size; };
	void bind() { glBindTexture(GL_TEXTURE_2D, this->handle); };
};
