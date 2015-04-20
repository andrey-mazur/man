#pragma once

#include <vector>
#include <string>

class manAudioDevice;
class manAudioDeviceList
{
public:
    manAudioDeviceList();

	std::vector<std::string> enumerateDevices();
	manAudioDevice * create(const std::string& name);
};
