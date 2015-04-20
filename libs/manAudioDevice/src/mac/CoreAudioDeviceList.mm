#import "CoreAudioDeviceList.h"

#import <AudioUnit/AudioUnit.h>
#import <AudioUnit/AudioComponent.h>
#import "CoreAudioDevice.h"


std::vector<std::string> CoreAudioDeviceList::enumerateDevices()
{
	AudioComponentDescription desc = {};
	desc.componentType = kAudioUnitType_Output;
	desc.componentSubType = kAudioUnitSubType_HALOutput;
	
	UInt32 devicesFound = AudioComponentCount(&desc);
	AudioComponent component = NULL;
	std::vector<std::string> v;
	for (UInt32 i = 0; i < devicesFound; ++i)
	{
		component = AudioComponentFindNext(component, &desc);
		CFStringRef componentName = NULL;
		AudioComponentCopyName(component, &componentName);
		const char * rawData = CFStringGetCStringPtr(componentName, kCFStringEncodingUTF8);
		if (rawData)
		{
			v.push_back(rawData);
		}
		CFRelease(componentName);
	}
	
	return v;
}

CoreAudioDevice * CoreAudioDeviceList::create(const std::string& name)
{
	CoreAudioDevice * device = new CoreAudioDevice();
	device->create(name);
	return device;
}
