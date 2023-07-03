#include <texture.h> 

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <log.h>
#include <stdexcept>

Texture::Texture(std::string filename) {
	stbi_set_flip_vertically_on_load(true);

	int width;
	int height;
	int channelCount;
	unsigned char* imageData = stbi_load(filename.c_str(), &width, &height, &channelCount, 0);
	if (imageData == NULL) {
		Log::getRendererLog()->error("Failed to load image: {} Error: {}", filename, stbi_failure_reason());
		throw std::exception("Failed to load image!");
	}

	this->size = glm::ivec2(width, height);
	
	// initialize the Texture
	glGenTextures(1, &this->handle);
	this->bind();

	// wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // shrinking linear or nearest
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // expanding

	int textureFormat = 0;
	switch (channelCount) {
	case 3:
		textureFormat = GL_RGB;
		break;
	case 4:
		textureFormat = GL_RGBA;
		break;
	default:
		Log::getRendererLog()->error("Failed to detect the format of texture: {}", filename);
		throw std::exception("Failed to detect texture format");
	}

	glTexImage2D(GL_TEXTURE_2D, 0, textureFormat, width, height, 0, textureFormat, GL_UNSIGNED_BYTE, imageData);
	
	stbi_image_free(imageData);
}

Texture::Texture(int width, int height) {
	throw std::exception("Texture::Texture(width, height) is unimplemented");
}