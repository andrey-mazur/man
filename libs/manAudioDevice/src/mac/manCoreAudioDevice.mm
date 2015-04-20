#import "CoreAudioDevice.h"

#import <AudioUnit/AudioUnit.h>


class CoreAudioDevicePrivate
{
public:
	CoreAudioDevicePrivate()
		: _audioUnit(NULL)
		, _audioFormat{}
	{
	}
	
	bool create(const std::string& name)
	{
		// TODO: remove copy-paste
		AudioComponentDescription desc = {};
		desc.componentType = kAudioUnitType_Output;
		desc.componentSubType = kAudioUnitSubType_HALOutput;
		
		UInt32 devicesFound = AudioComponentCount(&desc);
		AudioComponent component = NULL;
		for (UInt32 i = 0; i < devicesFound; ++i)
		{
			component = AudioComponentFindNext(component, &desc);
			CFStringRef componentName = NULL;
			AudioComponentCopyName(component, &componentName);
			const char * rawData = CFStringGetCStringPtr(componentName, kCFStringEncodingUTF8);
			if (name == rawData)
			{
				CFRelease(componentName);
				break;
			}
			CFRelease(componentName);
		}
		
		if (component)
		{
			AudioComponentInstanceNew(component, &_audioUnit);
			
			if (_audioUnit)
			{
				UInt32 size = sizeof(_audioFormat);
				AudioUnitGetProperty(_audioUnit,
									 kAudioUnitProperty_StreamFormat,
									 kAudioUnitScope_Output,
									 0,
									 &_audioFormat,
									 &size);
			}
		}
			
		return false;
	}
	
	AudioUnit _audioUnit;
	AudioStreamBasicDescription _audioFormat;
};

void CoreAudioDevice::create(const std::string& name)
{
	
}

void CoreAudioDevice::start()
{

}

void CoreAudioDevice::stop()
{
    
}
