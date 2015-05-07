#pragma once
#include <manAudioDevice/manAudioDevice.h>
#include <vector>

class manAudioEngine
{
public:
	explicit manAudioEngine();
	~manAudioEngine();

    void process(const manAudioBuffer inputBuffer, manAudioBuffer outputBuffer);
	void sinWave(const manAudioBuffer inputBuffer, manAudioBuffer outputBuffer);

private:
	manAudioDevice * _device;
	const std::vector<uint8_t> _buffer;
};
