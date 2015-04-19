#include <manAudioDevice/manAudioDeviceList.h>

#ifdef _WIN32
#include "pc/manAsioDevice.h"
#include <asiosdk/host/asiodrivers.h>
#endif

manAudioDeviceList::manAudioDeviceList()
{
}

std::vector<std::string> manAudioDeviceList::enumerateDevices()
{
	AsioDrivers driverList;

	const long maxDrivers = 8;
	char * names[maxDrivers] = {};
	for (long i = 0; i < maxDrivers; ++i)
	{
		names[i] = new char[MAXDRVNAMELEN];
	}
	const long driversFound = driverList.getDriverNames(names, maxDrivers);

	std::vector<std::string> v;
	v.reserve(driversFound);
	for (int i = 0; i < maxDrivers; ++i)
	{
		if (i < driversFound)
		{
			v.push_back(names[i]);
		}
		
		delete[] names[i];
	}

	return v;
}

manAudioDevice * manAudioDeviceList::create(const std::string& name)
{
	manAsioDevice * device = new manAsioDevice();
	device->create(name);
	return device;
}
