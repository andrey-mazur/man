#pragma once

class manTriangleWaveGenerator
{
public:
	manTriangleWaveGenerator();
    manTriangleWaveGenerator(float freq, float sampleRate);

	float frequency() const;
	void setFrequency(float freq);
	float sampleRate() const;
	void setSampleRate(float sampleRate);
    
	float value();

private:
	void updateDelta();

private:
    float _value;
	float _freq;
	float _sampleRate;
	float _delta;
};
