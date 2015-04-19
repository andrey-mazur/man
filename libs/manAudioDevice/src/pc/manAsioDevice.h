#pragma once

#include <manAudioDevice/manAudioDevice.h>
#include <string>

class manAsioDevicePrivate;
class manAsioDevice : public manAudioDevice
{
public:
    manAsioDevice();
	~manAsioDevice();

	void create(const std::string& name);
	void start();
	void stop();

private:
	manAsioDevicePrivate * _private;
};
