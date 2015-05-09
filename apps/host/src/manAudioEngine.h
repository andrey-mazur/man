#pragma once
#include <manAudioDevice/manAudioDevice.h>
#include "manSineWaveGenerator.h"
#include "manTriangleWaveGenerator.h"
#include "manSquareWaveGenerator.h"
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
	manSineWaveGenerator _sineWaveGen;
	manTriangleWaveGenerator _triangleWaveGen;
	manSquareWaveGenerator _squareWaveGen;
	manSineWaveGenerator _volumeSinGen;
	const std::vector<uint8_t> _buffer;
};
