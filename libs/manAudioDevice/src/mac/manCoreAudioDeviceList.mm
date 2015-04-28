#import "manCoreAudioDeviceList.h"

#import <CoreAudio/CoreAudio.h>
#import <locale>
#import <codecvt>
#import "manCoreAudioDevice.h"
#import "manCoreAudioHelper.h"


std::vector<std::string> manCoreAudioDeviceList::enumerateDevices()
{
	// TODO: remove duplicated code in manCoreAudioDevice.mm
	const AudioDeviceID device = kAudioObjectSystemObject;
	AudioObjectPropertyAddress theAddress = { kAudioHardwarePropertyDevices, kAudioObjectPropertyScopeGlobal, kAudioObjectPropertyElementMaster };

	UInt32 size = 0;
	AudioObjectGetPropertyDataSize(device, &theAddress, 0, NULL, &size);

	std::vector<std::string> v;
	if (size)
	{
		std::vector<AudioDeviceID> deviceIds;
		deviceIds.resize(size / sizeof(AudioDeviceID));
		AudioObjectGetPropertyData(device, &theAddress, 0, NULL, &size, &deviceIds.front());

		for (std::vector<AudioDeviceID>::const_iterator it = deviceIds.begin();
			 it != deviceIds.end(); ++it)
		{
			AudioDeviceID device = *it;

			theAddress.mSelector = kAudioDevicePropertyStreams;
			theAddress.mScope = kAudioDevicePropertyScopeOutput;
			AudioObjectGetPropertyDataSize(device, &theAddress, 0, NULL, &size);
			if (size)
			{
				CFStringRef deviceName = audioObjectGetPropertyData<CFStringRef>(device, kAudioObjectPropertyScopeGlobal, kAudioObjectPropertyName);
				const char * rawData = CFStringGetCStringPtr(deviceName, kCFStringEncodingUTF8);

				if (rawData)
				{
					v.push_back(rawData);
				}
				else
				{
					CFIndex length = CFStringGetLength(deviceName);
					if (length)
					{
						UniChar * buffer = new UniChar[length];
						CFRange range = { 0, length };
						CFStringGetCharacters(deviceName, range, buffer);

						std::u16string source = reinterpret_cast<char16_t *>(buffer);
						std::wstring_convert<std::codecvt_utf8_utf16<char16_t>,char16_t> convert;
						v.push_back(convert.to_bytes(source));

						delete [] buffer;
					}
				}
				CFRelease(deviceName);
			}
		}
	}

	return v;
}

manCoreAudioDevice * manCoreAudioDeviceList::create(const std::string& name)
{
	manCoreAudioDevice * device = new manCoreAudioDevice();
	device->create(name);
	return device;
}
