#include <manAudioDevice/manAudioDeviceList.h>

#ifdef _WIN32
#include "pc/manAsioDeviceList.h"
#include "pc/manAsioDevice.h"
#elif __APPLE__
#include "mac/manCoreAudioDevice.h"
#include "mac/manCoreAudioDeviceList.h"
#endif

manAudioDeviceList::manAudioDeviceList()
{
}

std::vector<std::string> manAudioDeviceList::enumerateDevices()
{
#ifdef _WIN32
	manAsioDeviceList deviceList;
	return deviceList.enumerateDevices();
#elif __APPLE__
	CoreAudioDeviceList deviceList;
	return deviceList.enumerateDevices();
#endif
}

manAudioDevice * manAudioDeviceList::create(const std::string& name)
{
#ifdef _WIN32
	manAsioDevice * device = new manAsioDevice();
	device->create(name);
	return device;
#elif __APPLE__
	CoreAudioDeviceList deviceList;
	return deviceList.create(name);
#endif
}
