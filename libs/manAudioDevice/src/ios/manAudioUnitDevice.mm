bool create(const std::string& name)
{
	AudioComponentDescription desc = {};
	desc.componentType = kAudioUnitType_Output;
	//desc.componentSubType = kAudioUnitSubType_HALOutput;
	desc.componentSubType = kAudioUnitSubType_DefaultOutput;

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

			AURenderCallbackStruct callback = {};
			callback.inputProc = audioRenderCallback;
			callback.inputProcRefCon = this;

			AudioUnitSetProperty(_audioUnit,
								 kAudioUnitProperty_SetRenderCallback,
								 kAudioUnitScope_Output,
								 0,
								 &callback,
								 sizeof(callback));

			AudioUnitInitialize(_audioUnit);

			return true;
		}
	}

	return false;
}

void start()
{
	AudioOutputUnitStart(_audioUnit);
}

void stop()
{
	AudioOutputUnitStop(_audioUnit);
}

AudioUnit _audioUnit;
AudioStreamBasicDescription _audioFormat;

OSStatus audioRenderCallback(void * inRefCon,
							 AudioUnitRenderActionFlags * ioActionFlags,
							 const AudioTimeStamp * inTimeStamp,
							 UInt32 inBusNumber,
							 UInt32 inNumberFrames,
							 AudioBufferList * ioData)
{
	manCoreAudioDevicePrivate * currentDevice = reinterpret_cast<manCoreAudioDevicePrivate *>(inRefCon);
	const float freq = 440.0;
	const float d = 2.0 * M_PI * freq / currentDevice->_audioFormat.mSampleRate;
	static float sinValue = 0.0;
	const float amplitude = 0.01;

	for (UInt32 i = 0; i < inNumberFrames; ++i)
	{
		float x = amplitude * sinf(sinValue) * std::numeric_limits<float>::max();
		sinValue += d;

		for (UInt32 bufferIndex = 0; bufferIndex < ioData->mNumberBuffers; ++bufferIndex)
		{
			AudioBuffer buffer = ioData->mBuffers[bufferIndex];
			float * ptr = reinterpret_cast<float *>(buffer.mData);
			ptr += i;

			for (UInt32 channel = 0; channel < buffer.mNumberChannels; ++channel)
			{
				*ptr = x;
				++ptr;
			}
		}
	}

	return noErr;
}
