#pragma once

#include <AL/alc.h>
#include <AL/al.h>

#include "source.h"
#include "buffer.h"

#include <memory>
#include <vector>
#include <unordered_map>

enum class Channel {
	Music,
	Effects
};

#ifdef DEBUG
#define AL_CHECK() Audio::checkError(__LINE__, __FILE__)
#define ALC_CHECK() Audio::get()->checkDeviceError(__LINE__, __FILE__)
#else
#define AL_CHECK() Audio::checkError(0, "release")
#define ALC_CHECK() Audio::get()->checkDeviceError(0, "release")
#endif

class Audio {
private:
	static Audio* singleton;
	static const uint32_t MAX_SOURCES = 32;

	ALCdevice* device;
	ALCcontext* context;

	float masterVolume = 1.0f;
	float musicVolume = 1.0f;
	float effectVolume = 1.0f;

	std::unique_ptr<Source> musicChannel;
	std::vector<std::unique_ptr<Source>> effectPool;
	std::unordered_map<std::string, std::unique_ptr<Buffer>> buffers;
public:
	Audio();
	~Audio();

	void effect(const std::string& id, float pitchVariation = 0.0f);
	void music(const std::string& id);

	// settings
	float getMasterVolume() { return masterVolume; };
	float getMusicVolume() { return musicVolume; };
	float getEffectVolume() { return effectVolume; };

	void setMasterVolume(float volume) { masterVolume = volume; alListenerf(AL_GAIN, masterVolume); }

	static bool checkError(int line, const char* filename);
	bool checkDeviceError(int line, const char* filename);

	static Audio* get() { if (!singleton) { new Audio(); } return singleton; };
};