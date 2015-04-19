#pragma once

#include <vector>

class manAudioDevice;
class manAudioDeviceList
{
public:
    manAudioDeviceList();

	std::vector<std::string> enumerateDevices();
	manAudioDevice * create(const std::string& name);
};
