#pragma once
#include <manAudioDevice/manAudioDevice.h>

class manAudioEngine
{
public:
	explicit manAudioEngine();
	~manAudioEngine();

    void process(const manAudioBuffer inputBuffer, manAudioBuffer outputBuffer);
	void sinWave(const manAudioBuffer inputBuffer, manAudioBuffer outputBuffer);

private:
	manAudioDevice * _device;
	SampleFormat _format;
	size_t _bytesPerSample;
	boost::function<void(float, uint8_t *)> _conversionFunction;
};
