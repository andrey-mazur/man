#include "manAsioDevice.h"

#include <asiosdk/host/asiodrivers.h>


manAsioDevice::manAsioDevice()
{
    AsioDrivers driverList;

	const long maxDrivers = 8;
	char * names[maxDrivers] = {};
	for (int i = 0; i < maxDrivers; ++i)
	{
		names[i] = new char[MAXDRVNAMELEN];
	}
	const long driversFound = driverList.getDriverNames(names, maxDrivers);

	for (int i = 0; i < maxDrivers; ++i)
	{
		delete[] names[i];
	}
}
