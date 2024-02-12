#pragma once

#include <AL/al.h>

#include "audio/buffer.h"

class Source {
private:
	ALuint handle;
public:
	Source();
	~Source();

	void play();
	void stop();
	bool isPlaying() const;
	void loop(bool should);
	bool isLooping() const;

	void setVolume(float volume);
	float getVolume() const;

	void setPitch(float pitch);
	float getPitch() const;

	void setBuffer(const Buffer* buffer);
};