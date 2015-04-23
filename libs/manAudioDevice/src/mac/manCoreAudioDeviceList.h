#pragma once

#include <vector>
#include <string>

class manCoreAudioDevice;
class manCoreAudioDeviceList
{
public:
	std::vector<std::string> enumerateDevices();
	
	manCoreAudioDevice * create(const std::string& name);
};
