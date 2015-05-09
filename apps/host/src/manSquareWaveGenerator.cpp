#include "manSquareWaveGenerator.h"

#include <math.h>


manSquareWaveGenerator::manSquareWaveGenerator()
	: manSquareWaveGenerator(0.0f, 0.0f)
{
}

manSquareWaveGenerator::manSquareWaveGenerator(float freq, float sampleRate)
	: _value(0.0f)
	, _freq(freq)
    , _sampleRate(sampleRate)
    , _delta(0.0f)
{
}

float manSquareWaveGenerator::frequency() const
{
	return _freq;
}

void manSquareWaveGenerator::setFrequency(float freq)
{
	_freq = freq;
	updateDelta();
}

float manSquareWaveGenerator::sampleRate() const
{
	return _sampleRate;
}

void manSquareWaveGenerator::setSampleRate(float sampleRate)
{
	_sampleRate = sampleRate;
	updateDelta();
}

float manSquareWaveGenerator::value()
{
	float ret = _value < static_cast<float>(M_PI) ? 1.0f : -1.0f;

	_value += _delta;
	if (_value > 2.0f * static_cast<float>(M_PI))
	{
		_value -= 2.0f * static_cast<float>(M_PI);
	}
	return ret;
}

void manSquareWaveGenerator::updateDelta()
{
	if (_sampleRate == 0.0f)
	{
		_delta = 0.0f;
	}
	else
	{
		_delta = 2.0f * static_cast<float>(M_PI)* _freq / _sampleRate;
	}
}
