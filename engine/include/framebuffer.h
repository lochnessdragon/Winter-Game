#pragma once

// System files
#include <cinttypes>

#include <glm/glm.hpp>

#include "GLAPI.h"
#include "texture.h"
#include "surface.h"

class Framebuffer : public Surface {
private:
	GLuint handle;
	glm::uvec2 size;

	GLuint colorBuffer;
	GLuint depthStencilRenderbuffer;
public:
	Framebuffer(uint32_t width, uint32_t height, TextureScaling scaling = TextureScaling::Nearest);

	void bind() { glBindFramebuffer(GL_FRAMEBUFFER, handle); };
	void unbind() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

	void bindColorBuffer() { glBindTexture(GL_TEXTURE_2D, colorBuffer); };

	glm::ivec2 getSize() { return size; }
};