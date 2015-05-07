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
	void start() override;
	void stop() override;
	void setAudioCallback(manAudioCallback) override;
	float sampleRate() override;
	SampleFormat sampleFormat() override;
	
private:
	manCoreAudioDevicePrivate * _private;
};
