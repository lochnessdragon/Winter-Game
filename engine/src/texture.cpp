#include <texture.h> 

#ifdef _MSC_VER
#pragma warning(disable: 4244) // Ignore C4244 coming from STB lib
#endif
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#ifdef _MSC_VER
#pragma warning(default: 4244) // Reverse ignore
#endif

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
		throw std::runtime_error("Failed to load image!");
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
		throw std::runtime_error("Failed to detect texture format");
	}

	glTexImage2D(GL_TEXTURE_2D, 0, textureFormat, width, height, 0, textureFormat, GL_UNSIGNED_BYTE, imageData);
	
	stbi_image_free(imageData);
}

Texture::Texture(TextureSpecification spec, const unsigned char* data) {
    this->size = glm::ivec2(spec.width, spec.height);

	GLenum scalingType = GL_LINEAR;
	if (spec.scaling == TextureScaling::Nearest)
		scalingType = GL_NEAREST;
    
    // create a new texture slot
    glGenTextures(1, &this->handle);
    this->bind();
    
    // wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // shrinking linear or nearest
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, scalingType); // expanding
    
    glTexImage2D(GL_TEXTURE_2D, 0, spec.format, spec.width, spec.height, 0, spec.format, GL_UNSIGNED_BYTE, data);
}

bool operator<(const Texture& lhs, const Texture& rhs) {
	return lhs.handle < rhs.handle;
}

bool operator==(const Texture& lhs, const Texture& rhs) {
	return lhs.handle == rhs.handle;
}