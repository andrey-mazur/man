#import "manCoreAudioDevice.h"

#import <CoreAudio/CoreAudio.h>
#import <math.h>
#import <vector>


class manCoreAudioDevicePrivate
{
public:
	manCoreAudioDevicePrivate()
	{
	}

	bool create(const std::string& name)
	{
		// TODO: remove duplicated code in manCoreAudioDeviceList.mm
		const AudioDeviceID device = kAudioObjectSystemObject;
		AudioObjectPropertyAddress theAddress = { kAudioObjectPropertyScopeGlobal, kAudioHardwarePropertyDevices, kAudioObjectPropertyElementMaster };

		UInt32 size = 0;
		AudioObjectGetPropertyDataSize(device, &theAddress, 0, NULL, &size);

		if (size)
		{
			std::vector<char> vData(size);
			AudioObjectGetPropertyData(device, &theAddress, 0, NULL, &size, &vData.front());
		}

		return false;
	}

	void start()
	{
	}

	void stop()
	{
	}
};


manCoreAudioDevice::manCoreAudioDevice()
{
	_private = new manCoreAudioDevicePrivate();
}

manCoreAudioDevice::~manCoreAudioDevice()
{
	delete _private;
}

void manCoreAudioDevice::create(const std::string& name)
{
	_private->create(name);
}

void manCoreAudioDevice::start()
{
	_private->start();
}

void manCoreAudioDevice::stop()
{
	_private->stop();
}
