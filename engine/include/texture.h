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
	Texture(std::string filename);
	~Texture() { glDeleteTextures(1, &this->handle); };

	const glm::ivec2& getSize() { return this->size; };
	void bind() { glBindTexture(GL_TEXTURE_2D, this->handle); };

	bool isTexture() { return glIsTexture(handle); };

protected:
	friend bool operator<(const Texture& lhs, const Texture& rhs);
	friend bool operator==(const Texture& lhs, const Texture& rhs);
};