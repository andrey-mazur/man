#pragma once


class manAudioUnitDevice;
class manAudioUnitDeviceList
{
public:
	std::vector<std::string> enumerateDevices();

	manCoreAudioDevice * create(const std::string& name);
};
