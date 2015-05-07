#pragma once
#include <manAudioDevice/manAudioDevice.h>

template<class T>
T audioObjectGetPropertyData(AudioDeviceID device, AudioObjectPropertyScope inScope, AudioDevicePropertyID inPropertyID, AudioObjectPropertyElement inElement = kAudioObjectPropertyElementMaster)
{
	AudioObjectPropertyAddress theAddress = { inPropertyID, inScope, inElement };

	UInt32 size = sizeof(T);
	T retVal;
	if (AudioObjectGetPropertyData(device, &theAddress, 0, NULL, &size, &retVal) == noErr)
	{
		return retVal;
	}

	return T{0};
}
