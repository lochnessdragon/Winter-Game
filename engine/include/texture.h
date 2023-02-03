#pragma once

#include <string>

#include "GLAPI.h"

#include <glm/glm.hpp>

class Texture {
private:
	GLuint handle;

public:
	Texture(int width, int height);
	Texture(std::string& filename);
	~Texture();

	void setData();
	glm::vec2i getSize();
	void bind() { glBindTexture(GL_TEXTURE_2D, this->handle)};
}