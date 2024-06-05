#include "audio/audio.h"

#include <cstdlib>
#include <cmath>

#include <log.h>

Audio* Audio::singleton = nullptr;

Audio::Audio() {
	Audio::singleton = this;

	Log::getGameLog()->info("Initializing audio system.");

	// print openal debug info
	if (alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT") == ALC_TRUE) {
		const ALCchar* devices = alcGetString(NULL, ALC_DEVICE_SPECIFIER);
		const ALCchar* currentName = devices;
		while (strlen(currentName) != 0) {
			Log::getGameLog()->info("Open AL device found: {}", currentName);
			currentName += strlen(currentName) + 1;
		}
	}

	device = alcOpenDevice(NULL);
	if (!device) {
		Log::getGameLog()->critical("Failed to create open al device.");
		throw std::runtime_error("Failed to make open al device");
	}

	context = alcCreateContext(device, NULL);
	alcMakeContextCurrent(context);

	checkDeviceError(__LINE__, __FILE__); // clear error code
	AL_CHECK();

	// query for open al information

	// initialize music source and object pool
	musicChannel = std::make_unique<Source>();
	for (int i = 0; i < MAX_SOURCES; i++) {
		effectPool.push_back(std::make_unique<Source>());
	}
}

Audio::~Audio()
{
	alcMakeContextCurrent(NULL);
	alcDestroyContext(context);
	alcCloseDevice(device);
}

bool Audio::checkError(int line, const char* filename) {
	ALenum error = alGetError();
	switch (error) {
		case AL_INVALID_NAME:
			// a bad name(ID) was passed to an OpenAL function
			Log::getGameLog()->warn("OpenAL Error: AL_INVALID_NAME, a bad name(ID) was passed to an OpenAL function. check before {}:{}", filename, line);
			break;
		case AL_INVALID_ENUM:
			// an invalid enum value was passed to an OpenAL function
			Log::getGameLog()->warn("OpenAL Error: AL_INVALID_ENUM, an invalid enum value was passed to an OpenAL function. check before {}:{}", filename, line);
			break;
		case AL_INVALID_VALUE:
			// an invalid value was passed to an OpenAL function
			Log::getGameLog()->warn("OpenAL Error: AL_INVALID_VALUE, an invalid value was passed to an OpenAL function. check before {}:{}", filename, line);
			break;
		case AL_INVALID_OPERATION:
			// the requested operation is not valid
			Log::getGameLog()->warn("OpenAL Error: AL_INVALID_OPERATION, the requested operation is not valid. check before {}:{}", filename, line);
			break;
		case AL_OUT_OF_MEMORY:
			// the request operation resulted in OpenAL running out of memory
			Log::getGameLog()->warn("OpenAL Error: AL_OUT_OF_MEMORY, the request operation resulted in OpenAL running out of memory. check before {}:{}", filename, line);
			break;
		default:
			return false;
			break; // no error
	}

	return true;
}

bool Audio::checkDeviceError(int line, const char* filename)
{
	ALCenum error = alcGetError(device);
	switch (error) {
		case ALC_INVALID_DEVICE:
			// a bad device was passed to an OpenAL function
			Log::getGameLog()->warn("ALC Error: ALC_INVALID_DEVICE, a bad device was passed to an OpenAL function. check before {}:{}", filename, line);
			break;
		case ALC_INVALID_CONTEXT:
			// a bad context was passed to an OpenAL function
			Log::getGameLog()->warn("ALC Error: ALC_INVALID_CONTEXT, a bad context was passed to an OpenAL function. check before {}:{}", filename, line);
			break;
		case ALC_INVALID_ENUM:
			// an unknown enum value was passed to an OpenAL function
			Log::getGameLog()->warn("ALC Error: ALC_INVALID_ENUM, an unknown enum value was passed to an OpenAL function. check before {}:{}", filename, line);
			break;
		case ALC_INVALID_VALUE:
			// an invalid value was passed to an OpenAL function
			Log::getGameLog()->warn("ALC Error: ALC_INVALID_VALUE, an invalid value was passed to an OpenAL function. check before {}:{}", filename, line);
			break;
		case ALC_OUT_OF_MEMORY:
			// the requested operation resulted in OpenAL running out of memory
			Log::getGameLog()->warn("ALC Error: ALC_OUT_OF_MEMORY, the requested operation resulted in OpenAL running out of memory. check before {}:{}", filename, line);
			break;
		default:
			return false;
			break;
	}
	return true;
}

float genRandomFloat() {
	return ((float) std::rand() / (float) (RAND_MAX / 2)) - 1.0f;
}

void Audio::effect(const std::string& id, float pitchVariance) {
	if (buffers.count(id) <= 0) {
		// doesn't exist
		buffers.insert({id, std::make_unique<Buffer>("res/audio/effects/" + id + ".wav") });
	}

	float pitch = 1.0f + (genRandomFloat() * pitchVariance);
	pitch = std::max(0.0f, pitch);

	for (int i = 0; i < effectPool.size(); i++) {
		if (!effectPool[i]->isPlaying()) {
			// found one
			effectPool[i]->setBuffer(buffers.at(id).get());
			effectPool[i]->setVolume(effectVolume);
			effectPool[i]->setPitch(pitch);
			effectPool[i]->play();
			break;
		}
	}
}
