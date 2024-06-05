#include "audio/source.h"

#include "audio/audio.h"
#include "log.h"

Source::Source() {
	alGenSources(1, &handle);
	AL_CHECK();
}

Source::~Source() {
	alDeleteSources(1, &handle);
}

void Source::play() {
	alSourcePlay(handle);
}

void Source::stop() {
	alSourceStop(handle);
}

bool Source::isPlaying() const {
	ALint state = 0;
	alGetSourcei(handle, AL_SOURCE_STATE, &state);
	return state == AL_PLAYING;
}

void Source::loop(bool should) {
	alSourcei(handle, AL_LOOPING, should);
}

bool Source::isLooping() const {
	ALint looping = false;
	alGetSourcei(handle, AL_LOOPING, &looping);
	return looping;
}

void Source::setVolume(float volume) {
	alSourcef(handle, AL_GAIN, volume);
}

float Source::getVolume() const {
	ALfloat volume = 0.0f;
	alGetSourcef(handle, AL_GAIN, &volume);
	return volume;
}

void Source::setPitch(float pitch) {
	alSourcef(handle, AL_PITCH, pitch);
}

float Source::getPitch() const {
	float pitch = 0.0f;
	alGetSourcef(handle, AL_PITCH, &pitch);
	return pitch;
}

void Source::setBuffer(const Buffer* buffer) {
	alSourcei(handle, AL_BUFFER, buffer->getHandle());
}