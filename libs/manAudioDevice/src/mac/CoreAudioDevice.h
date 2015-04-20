#pragma once

#include <manAudioDevice/manAudioDevice.h>
#include <string>

class CoreAudioDevice : public manAudioDevice
{
public:
	void create(const std::string& name);
	void start();
	void stop();
};
