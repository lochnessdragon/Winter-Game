#pragma once

#include <string>

#include <AL/al.h>

class Buffer {
private:
	ALuint handle;
public:
	Buffer(const std::string& filename);
	~Buffer();

	ALuint getHandle() const { return handle; };
};