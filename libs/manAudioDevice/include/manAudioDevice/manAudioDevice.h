#pragma once

class manAudioDevice
{
public:
	virtual ~manAudioDevice();

	virtual void start() = 0;
	virtual void stop() = 0;
};
