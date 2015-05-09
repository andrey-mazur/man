#include "manSineWaveGenerator.h"

#include <math.h>


manSineWaveGenerator::manSineWaveGenerator()
	: manSineWaveGenerator(0.0f, 0.0f)
{
}

manSineWaveGenerator::manSineWaveGenerator(float freq, float sampleRate)
	: _value(0.0f)
	, _freq(freq)
	, _sampleRate(sampleRate)
{
	updateDelta();
}

float manSineWaveGenerator::frequency() const
{
	return _freq;
}

void manSineWaveGenerator::setFrequency(float freq)
{
	_freq = freq;
	updateDelta();
}

float manSineWaveGenerator::sampleRate() const
{
	return _sampleRate;
}

void manSineWaveGenerator::setSampleRate(float sampleRate)
{
	_sampleRate = sampleRate;
	updateDelta();
}

float manSineWaveGenerator::value()
{
	float ret = sinf(_value);
	_value += _delta;
	if (_value > 2.0f * static_cast<float>(M_PI))
	{
		_value -= 2.0f * static_cast<float>(M_PI);
	}
	return ret;
}

void manSineWaveGenerator::updateDelta()
{
	if (_sampleRate == 0.0f)
	{
		_delta = 0.0f;
	}
	else
	{
		_delta = 2.0f * static_cast<float>(M_PI) * _freq / _sampleRate;
	}
}
