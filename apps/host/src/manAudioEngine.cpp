#include "manAudioEngine.h"
#include <boost/bind.hpp>
#include <stdint.h>
#include <iostream>
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
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
	const float freq1 = 440.0f;
	const float d1 = 2.0f * static_cast<float>(M_PI) * freq1 / static_cast<float>(_device->sampleRate());
	static float sinValue1 = 0.0f;
	const float freq2 = 330.0f;
	const float d2 = 2.0f * static_cast<float>(M_PI) * freq2 / static_cast<float>(_device->sampleRate());
	static float sinValue2 = 0.0f;
	float amplitude = 0.12f;

	for (long j = 0; j < bufferSize; ++j)
	{
		float value = amplitude * sin(sinValue1);

		for (long i = 0; i < outputBuffer.numChannels; ++i)
		{
			if (inputBuffer.numChannels)
			{
				float * inputPtr = inputBuffer.data[i];
				inputPtr += j;
			}

			float * outputPtr = outputBuffer.data[i];
			outputPtr += j;
			
			value -= amplitude * sin(sinValue2);
			*outputPtr = value;
		}
		sinValue1 += d1;
		sinValue2 += d2;
	}
}
