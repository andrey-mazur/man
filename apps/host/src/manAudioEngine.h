#pragma once
#include <manAudioDevice/manAudioDevice.h>

class manAudioEngine
{
public:
	explicit manAudioEngine();

    void process(const manAudioBuffer inputBuffer, manAudioBuffer outputBuffer);
	
	template <typename T>
	void sinWave(const manAudioBuffer inputBuffer, manAudioBuffer outputBuffer);

private:
	manAudioDevice * _device;
};
