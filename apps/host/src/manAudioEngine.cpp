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


template <typename T>
void convert(float value, uint8_t * buffer)
{
	*(reinterpret_cast<T *>(buffer)) = static_cast<T>(value * std::numeric_limits<T>::max());
}

template <>
void convert<float>(float value, uint8_t * buffer)
{
	*(reinterpret_cast<float *>(buffer)) = static_cast<float>(value);
}

static size_t getBytesPerSample(SampleFormat format)
{
	switch (format)
	{
	case SampleFormat_Int16:
		return sizeof(int16_t);
	case SampleFormat_Int32:
		return sizeof(int32_t);
	case SampleFormat_Float32:
		return sizeof(float);
	}

	return 0;
}

static boost::function<void(float, uint8_t *)> getConversionFunction(SampleFormat format)
{
	switch (format)
	{
	case SampleFormat_Int16:
		return &convert<int16_t>;
	case SampleFormat_Int32:
		return &convert<int32_t>;
	case SampleFormat_Float32:
		return &convert<float>;
	}

	return &convert<float>;
}

manAudioEngine::manAudioEngine()
	: _device(nullptr)
	, _format(SampleFormat_Unknown)
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
	_format = _device->sampleFormat();
	_bytesPerSample = getBytesPerSample(_format);
	_conversionFunction = getConversionFunction(_format);
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
	const size_t bufferSize = outputBuffer.numLengthInBytes / outputBuffer.numChannels / _bytesPerSample;
	const float freq1 = 440.0f;
	const float d1 = 2.0f * static_cast<float>(M_PI) * freq1 / static_cast<float>(_device->sampleRate());
	static float sinValue1 = 0.0f;
	const float freq2 = 330.0f;
	const float d2 = 2.0f * static_cast<float>(M_PI) * freq2 / static_cast<float>(_device->sampleRate());
	static float sinValue2 = 0.0f;
	float amplitude = 0.02f;

	for (long j = 0; j < bufferSize; ++j)
	{
		float value = amplitude * sin(sinValue1);

		for (long i = 0; i < outputBuffer.numChannels; ++i)
		{
			uint8_t * inputPtr = reinterpret_cast<uint8_t *>(inputBuffer.data[i]);
			inputPtr += j * _bytesPerSample;

			uint8_t * outputPtr = reinterpret_cast<uint8_t *>(outputBuffer.data[i]);
			outputPtr += j * _bytesPerSample;
			
			value -= amplitude * sin(sinValue2);
			_conversionFunction(value, outputPtr);
		}
		sinValue1 += d1;
		sinValue2 += d2;
	}
}
