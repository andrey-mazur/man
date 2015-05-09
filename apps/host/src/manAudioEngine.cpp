#include "manAudioEngine.h"
#include <boost/bind.hpp>
#include <stdint.h>
#include <iostream>
#include <math.h>
#include <manAudioDevice/manAudioDeviceList.h>
#include <manAudioDevice/manAudioDevice.h>


manAudioEngine::manAudioEngine()
	: _device(nullptr)
	, _buffer(sizeof(float) * 10000, 0)
{
	manAudioDeviceList list;
	std::vector<std::string> deviceList = list.enumerateDevices();
	std::for_each(deviceList.begin(), deviceList.end(),
		[](std::string& element){ std::cout << element << std::endl; });
#ifdef __APPLE__
	std::reverse(deviceList.begin(), deviceList.end());
#endif

	_device = list.create(deviceList.front());
	_device->setAudioCallback(boost::bind(&manAudioEngine::process, this, _1, _2));

	float sampleRate = _device->sampleRate();
	_sineWaveGen.setFrequency(440.0f);
	_sineWaveGen.setSampleRate(sampleRate);
	_triangleWaveGen.setFrequency(660.0f);
	_triangleWaveGen.setSampleRate(sampleRate);
	_squareWaveGen.setFrequency(770.0f);
	_squareWaveGen.setSampleRate(sampleRate);
	_volumeSinGen.setFrequency(0.5f);
	_volumeSinGen.setSampleRate(sampleRate);

	_device->start();
}

manAudioEngine::~manAudioEngine()
{
	delete _device;
}

void manAudioEngine::process(const manAudioBuffer inputBuffer, manAudioBuffer outputBuffer)
{
	sinWave(inputBuffer, outputBuffer);
}

void manAudioEngine::sinWave(const manAudioBuffer inputBuffer, manAudioBuffer outputBuffer)
{
	const size_t bufferSize = outputBuffer.numLengthInBytes / outputBuffer.numChannels / sizeof(float);
	const float amplitude = 0.12f;

	for (long j = 0; j < bufferSize; ++j)
	{
		float value = amplitude * (_sineWaveGen.value() + _triangleWaveGen.value()); // +_squareWaveGen.value());
		float vol = fabs(_volumeSinGen.value());

		for (long i = 0; i < outputBuffer.numChannels; ++i)
		{
			float * outputPtr = outputBuffer.data[i];
			outputPtr += j;

			if (inputBuffer.numChannels)
			{
				float * inputPtr = inputBuffer.data[i];
				inputPtr += j;
				//value = *inputPtr;
			}
			
			value *= vol;
			*outputPtr = value;
		}
	}
}
