#pragma once

#include <vector>
#include <string>

class CoreAudioDevice;
class CoreAudioDeviceList
{
public:
	std::vector<std::string> enumerateDevices();
	
	CoreAudioDevice * create(const std::string& name);
};
