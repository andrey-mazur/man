#include "manTriangleWaveGenerator.h"

#include <math.h>


manTriangleWaveGenerator::manTriangleWaveGenerator()
	: manTriangleWaveGenerator(0.0f, 0.0f)
{
}

manTriangleWaveGenerator::manTriangleWaveGenerator(float freq, float sampleRate)
	: _value(0.0f)
	, _freq(freq)
    , _sampleRate(sampleRate)
    , _delta(0.0f)
{
}

float manTriangleWaveGenerator::frequency() const
{
	return _freq;
}

void manTriangleWaveGenerator::setFrequency(float freq)
{
	_freq = freq;
	updateDelta();
}

float manTriangleWaveGenerator::sampleRate() const
{
	return _sampleRate;
}

void manTriangleWaveGenerator::setSampleRate(float sampleRate)
{
	_sampleRate = sampleRate;
	updateDelta();
}

float manTriangleWaveGenerator::value()
{
	float ret = 0.0f;
    if (_value < M_PI)
    {
		ret = -1.0f + (2.0f / static_cast<float>(M_PI)) * _value;
    }
    else
    {
		ret = 3.0f - (2.0f / static_cast<float>(M_PI)) * _value;
    }

	_value += _delta;
    if (_value > 2.0f * static_cast<float>(M_PI))
    {
		_value -= 2.0f * static_cast<float>(M_PI);
    }

	return ret;
}

void manTriangleWaveGenerator::updateDelta()
{
	if (_sampleRate == 0.0f)
	{
		_delta = 0.0f;
	}
	else
	{
		_delta = (2.0f * static_cast<float>(M_PI) * _freq) / _sampleRate;
	}
}
