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
{
	manAudioDeviceList list;
	std::vector<std::string> deviceList = list.enumerateDevices();
	std::for_each(deviceList.begin(), deviceList.end(),
		[](std::string& element){ std::cout << element << std::endl; });
#ifdef Q_OS_MAC
	std::reverse(deviceList.begin(), deviceList.end());
#endif

	_device = list.create(deviceList.front());
	_device->setAudioCallback(boost::bind(&manAudioEngine::process, this, _1, _2));
	_device->start();
}

void manAudioEngine::process(const manAudioBuffer inputBuffer, manAudioBuffer outputBuffer)
{
#ifdef Q_OS_MAC
	typedef float SampleType;
#else
	typedef int32_t SampleType;
#endif
	sinWave<SampleType>(inputBuffer, outputBuffer);
}

template <typename T>
void manAudioEngine::sinWave(const manAudioBuffer inputBuffer, manAudioBuffer outputBuffer)
{
	const long bufferSize = outputBuffer.numLengthInBytes / outputBuffer.numChannels / sizeof(T);
	const float freq = 440.0f;
	const float d = 2.0f * static_cast<float>(M_PI) * freq / static_cast<float>(_device->sampleRate());
	static float sinValue = 0.0f;
	float amplitude = 0.02f;

	for (long j = 0; j < bufferSize; ++j)
	{
		T value = static_cast<T>(amplitude * sin(sinValue) * std::numeric_limits<T>::max());

		for (long i = 0; i < outputBuffer.numChannels; ++i)
		{
			T * inputPtr = reinterpret_cast<T *>(inputBuffer.data);
			inputPtr += i;
			inputPtr += j;

			T * outputPtr = reinterpret_cast<T *>(outputBuffer.data);
			outputPtr += i;
			outputPtr += j;
			*outputPtr = value;
			//*outputPtr += *inputPtr;
		}
		sinValue += d;
	}
}
