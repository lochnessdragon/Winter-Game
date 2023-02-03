#include <texture.h> 

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <log.h>
#include <stdexcept>

Texture::Texture(std::string& filename) {
	int width;
	int height;
	int channelCount;
	unsigned char* imageData = stbi_load(filename.c_str(), &width, &height, &channelCount, 0);
	if (imageData == NULL) {
		Log::getRendererLog()->error("Failed to load image: {} Error: {}", filename, stbi_failure_reason());
		throw std::exception("Failed to load image!");
	}
	// initialize the Texture
	glGenTextures(1, &this->handle);
	this->bind();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
	
	
	stbi_image_free(imageData);
}

Texture::Texture(int width, int height) {
	throw std::exception("Texture::Texture(width, height) is unimplemented");
}