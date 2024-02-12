#include "audio/buffer.h"

#include "audio/audio.h"
#include "log.h"

#include "audio/AudioFile.h"

Buffer::Buffer(const std::string& filename) {
	alGenBuffers(1, &handle);
	AL_CHECK();

	AudioFile<int16_t> file(filename);
	int bitsPerSample = file.getBitDepth();
	ALenum format;
	if (file.isMono()) {
		// mono
		if (bitsPerSample == 8) {
			format = AL_FORMAT_MONO8;
		} else {
			format = AL_FORMAT_MONO16;
		}
	}
	else {
		// stereo
		if (bitsPerSample == 8) {
			format = AL_FORMAT_STEREO8;
		}
		else {
			format = AL_FORMAT_STEREO16;
		}
	}

	Log::getGameLog()->info("Loading buffer from file: {}. channels = {} bitsPerSample = {} samples = {}", filename, file.getNumChannels(), bitsPerSample, file.getNumSamplesPerChannel());

	/*
	* 8-bit PCM data is expressed as an unsigned value over the range 0 to 255, 128 being an
	* audio output level of zero. 16-bit PCM data is expressed as a signed value over the
	* range -32768 to 32767, 0 being an audio output level of zero. Stereo data is expressed
	* in interleaved format, left channel first. Buffers containing more than one channel of data
	* will be played without 3D spatialization.
	*/
	// convert from library format to openal format
	std::vector<char> soundBuffer(file.getNumSamplesPerChannel() * file.getNumChannels() * (bitsPerSample == 8 ? 1 : 2));
	if (bitsPerSample == 8) {
		// heavy conversion required (int16 -> uint8) [-127, 127] -> [0, 255]
		for (int sample = 0; sample < file.getNumSamplesPerChannel(); sample++) {
			for (int channel = 0; channel < file.getNumChannels(); channel++) {
				soundBuffer[(sample * file.getNumChannels()) + channel] = (char) (file.samples[channel][sample] + 128);
			}
		}
	}
	else {
		// limited conversion required, interleave data.
		for (int sample_idx = 0; sample_idx < file.getNumSamplesPerChannel(); sample_idx++) {
			for (int channel = 0; channel < file.getNumChannels(); channel++) {
				int16_t sample = file.samples[channel][sample_idx];
				size_t buff_idx = ((sample_idx * file.getNumChannels()) + channel) * 2; // 2 for the two parts
				soundBuffer[buff_idx] = (char) (sample) & 0xff; // first half
				soundBuffer[buff_idx + 1] = (char) (sample >> 8) & 0xff; // second half
			}
		}
	}

	alBufferData(handle, format, soundBuffer.data(), soundBuffer.size(), file.getSampleRate());
	AL_CHECK();
}

Buffer::~Buffer() {
	alDeleteBuffers(1, &handle);
}