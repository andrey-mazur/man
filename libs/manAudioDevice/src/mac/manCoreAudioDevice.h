#pragma once

#include <manAudioDevice/manAudioDevice.h>
#include <string>

class manCoreAudioDevicePrivate;
class manCoreAudioDevice : public manAudioDevice
{
public:
	manCoreAudioDevice();
	~manCoreAudioDevice();
	
	void create(const std::string& name);
	void start();
	void stop();
	
private:
	manCoreAudioDevicePrivate * _private;
};
