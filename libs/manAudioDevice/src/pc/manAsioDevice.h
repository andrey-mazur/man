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
	void start() override;
	void stop() override;
	void setAudioCallback(manAudioCallback) override;
	float sampleRate() override;
	SampleFormat sampleFormat() override;

private:
	manAsioDevicePrivate * _private;
};
