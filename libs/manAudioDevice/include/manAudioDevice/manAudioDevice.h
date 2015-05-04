#pragma once
#include <boost/function.hpp>

typedef boost::function<void(void **, long, void **, long)> manAudioCallback;

class manAudioDevice
{
public:
	virtual ~manAudioDevice();

	virtual void start() = 0;
	virtual void stop() = 0;
	virtual void setAudioCallback(manAudioCallback) = 0;
	virtual float sampleRate() = 0;
	virtual long bufferSize() = 0;
};
