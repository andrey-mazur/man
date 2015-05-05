#import "manCoreAudioDevice.h"

#import <CoreAudio/CoreAudio.h>
#import <math.h>
#import <vector>
#import <locale>
#import <codecvt>
#import "manCoreAudioHelper.h"

OSStatus AudioDeviceIO(AudioObjectID     inDevice,
						   const AudioTimeStamp*   inNow,
						   const AudioBufferList*  inInputData,
						   const AudioTimeStamp*   inInputTime,
						   AudioBufferList*        outOutputData,
						   const AudioTimeStamp*   inOutputTime,
						   void*                   inClientData);

class manCoreAudioDevicePrivate
{
public:
	manCoreAudioDevicePrivate()
	: deviceId(0)
	, procId(0)
	, streamFormat{0}
	{
	}
	
	~manCoreAudioDevicePrivate()
	{
		AudioDeviceDestroyIOProcID(deviceId, procId);
	}

	bool create(const std::string& name)
	{
		// TODO: remove duplicated code in manCoreAudioDeviceList.mm
		const AudioDeviceID device = kAudioObjectSystemObject;
		AudioObjectPropertyAddress theAddress = { kAudioHardwarePropertyDevices, kAudioObjectPropertyScopeGlobal, kAudioObjectPropertyElementMaster };

		UInt32 size = 0;
		AudioObjectGetPropertyDataSize(device, &theAddress, 0, NULL, &size);

		if (size)
		{
			std::vector<AudioDeviceID> deviceIds;
			deviceIds.resize(size / sizeof(AudioDeviceID));
			AudioObjectGetPropertyData(device, &theAddress, 0, NULL, &size, &deviceIds.front());
			
			AudioDeviceID device = 0;
			for (std::vector<AudioDeviceID>::const_iterator it = deviceIds.begin();
				 it != deviceIds.end(); ++it)
			{
				device = *it;
				theAddress.mSelector = kAudioObjectPropertyName;
				theAddress.mScope = kAudioObjectPropertyScopeGlobal;
				size = sizeof(CFStringRef);
				CFStringRef deviceNameStringRef = audioObjectGetPropertyData<CFStringRef>(device, kAudioObjectPropertyScopeGlobal, kAudioObjectPropertyName);
				
				const char * rawData = CFStringGetCStringPtr(deviceNameStringRef, kCFStringEncodingUTF8);
				
				std::string deviceName;
				if (rawData)
				{
					deviceName = rawData;
				}
				else
				{
					CFIndex length = CFStringGetLength(deviceNameStringRef);
					if (length)
					{
						UniChar * buffer = new UniChar[length];
						CFRange range = { 0, length };
						CFStringGetCharacters(deviceNameStringRef, range, buffer);
						
						std::u16string source = reinterpret_cast<char16_t *>(buffer);
						std::wstring_convert<std::codecvt_utf8_utf16<char16_t>,char16_t> convert;
						deviceName = convert.to_bytes(source);
						
						delete [] buffer;
					}
				}
				
				CFRelease(deviceNameStringRef);
				
				if (deviceName == name)
					break;
			}
			
			if (device > 0)
			{
				deviceId = device;
				
				theAddress.mSelector = kAudioDevicePropertyStreamFormat;
				theAddress.mScope = kAudioObjectPropertyScopeOutput;
				AudioObjectGetPropertyDataSize(device, &theAddress, 0, NULL, &size);
				
				streamFormat = audioObjectGetPropertyData<AudioStreamBasicDescription>(device, kAudioObjectPropertyScopeOutput, kAudioDevicePropertyStreamFormat);
				
				AudioDeviceCreateIOProcID(device, AudioDeviceIO, this, &procId);
			}
		}

		return false;
	}

	void start()
	{
		AudioDeviceStart(deviceId, procId);
	}

	void stop()
	{
		AudioDeviceStop(deviceId, procId);
	}
	
	void setCallback(manAudioCallback callback)
	{
		audioCallback = callback;
	}
	
	float sampleRate() const
	{
		return streamFormat.mSampleRate;
	}
	
	AudioDeviceID deviceId;
	AudioDeviceIOProcID procId;
	AudioStreamBasicDescription streamFormat;
	manAudioCallback audioCallback;
};

OSStatus AudioDeviceIO(AudioObjectID     inDevice,
					 const AudioTimeStamp*   inNow,
					 const AudioBufferList*  inInputData,
					 const AudioTimeStamp*   inInputTime,
					 AudioBufferList*        outOutputData,
					 const AudioTimeStamp*   inOutputTime,
					 void*                   inClientData)
{
	manCoreAudioDevicePrivate * privatePart = reinterpret_cast<manCoreAudioDevicePrivate *>(inClientData);
	if (privatePart->audioCallback)
	{
		manAudioBuffer inputBuffer = {};
		if (inInputData->mNumberBuffers)
		{
			inputBuffer = {static_cast<int>(inInputData->mBuffers->mNumberChannels),
				static_cast<int>(inInputData->mBuffers->mDataByteSize), inInputData->mBuffers->mData};
		}
		
		manAudioBuffer outputBuffer = {};
		if (outOutputData->mNumberBuffers)
		{
			outputBuffer = {static_cast<int>(outOutputData->mBuffers->mNumberChannels),
				static_cast<int>(outOutputData->mBuffers->mDataByteSize), outOutputData->mBuffers->mData};
		}
		privatePart->audioCallback(inputBuffer, outputBuffer);
	}
	/*
	const float freq = 440.0f;
	const float d = 2.0f * M_PI * freq / privatePart->streamFormat.mSampleRate;
	static float sinValue = 0.0f;
	const float amplitude = 0.02f;
	
	for (UInt32 i = 0; i < outOutputData->mNumberBuffers; ++i)
	{
		float value = amplitude * sinf(sinValue) * std::numeric_limits<float>::max();
		const double bufferSize = outOutputData->mBuffers[i].mDataByteSize / sizeof(float);
		for (UInt32 j = 0; j < bufferSize;)
		{
			float * ptr = reinterpret_cast<float *>(outOutputData->mBuffers[i].mData);
			ptr += j;
			
			for (UInt32 channel = 0; channel < outOutputData->mBuffers[i].mNumberChannels; ++channel)
			{
				*(ptr + channel) = value;
				
				++j;
			}
		}
		sinValue += d;
	}
	 */
	
	return noErr;
}


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

void manCoreAudioDevice::setAudioCallback(manAudioCallback callback)
{
	_private->setCallback(callback);
}

float manCoreAudioDevice::sampleRate()
{
	return _private->sampleRate();
}
