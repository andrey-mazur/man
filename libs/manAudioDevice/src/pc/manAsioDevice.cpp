#include "manAsioDevice.h"

#include <asiosdk/common/asiosys.h>
#include <asiosdk/common/asio.h>
#include <asiosdk/host/asiodrivers.h>
#include <asiosdk/host/ASIOConvertSamples.h>


/* asio callbacks declarations */
void asio_bufferSwitch(long index, ASIOBool directProcess);
void asio_sampleRateDidChange(ASIOSampleRate sampleRate);
long asio_message(long selector, long value, void * message, double * opt);
ASIOTime * asio_bufferSwitchTimeInfo(ASIOTime * params, long index, ASIOBool directProcess);

const float kInt32ToFloatMultiplier = 1.0 / 0x7fffffff;
inline void fromInt32ToFloatInPlace(void * buffer, int samples)
{
	const float kScaler32Reverse = 1.0 / 0x7fffffff;

	int * intBuffer = static_cast<int *>(buffer);
	float * floatBuffer = static_cast<float *>(buffer);
	while (--samples >= 0)
	{
		*floatBuffer++ = static_cast<float>(kInt32ToFloatMultiplier * (*intBuffer++));
	}
}

manAsioDevicePrivate * currentDevice = nullptr;
class manAsioDevicePrivate
{
public:
	manAsioDevicePrivate()
		: callbacks{}
		, driverInfo{}
		, numInputChannels(0)
		, numOutputChannels(0)
		, minBufferSize(0)
		, maxBufferSize(0)
		, preferredBufferSize(0)
		, granularity(0)
		, asioOutputReady(false)
		, bufferInfos(nullptr)
		, sampleRate{}
		, input(nullptr)
		, output(nullptr)
	{
		callbacks.bufferSwitch = asio_bufferSwitch;
		callbacks.sampleRateDidChange = asio_sampleRateDidChange;
		callbacks.asioMessage = asio_message;
		callbacks.bufferSwitchTimeInfo = asio_bufferSwitchTimeInfo;
		driverInfo.asioVersion = 2;
	}

	~manAsioDevicePrivate()
	{
		stop();
		close();
	}

	bool create(const std::string& name)
	{
		if (driverList.loadDriver(const_cast<char *>(name.c_str())))
		{
			currentDevice = this;
			return true;
		}

		return false;
	}

	void init()
	{
		ASIOInit(&driverInfo);

		ASIOGetChannels(&numInputChannels, &numOutputChannels);

		ASIOGetBufferSize(&minBufferSize, &maxBufferSize, &preferredBufferSize, &granularity);

		ASIOGetSampleRate(&sampleRate);

		asioOutputReady = ASIOOutputReady() == ASE_OK;

		if (bufferInfos)
		{
			ASIODisposeBuffers();
			delete[] bufferInfos;
		}
		bufferInfos = new ASIOBufferInfo[numInputChannels + numOutputChannels];
		ASIOBufferInfo * bufferDesc = bufferInfos;
		channelInfos = new ASIOChannelInfo[numInputChannels + numOutputChannels];
		ASIOChannelInfo * channelDesc = channelInfos;
		for (long i = 0; i < numInputChannels; i++, bufferDesc++, channelDesc++)
		{
			bufferDesc->isInput = ASIOTrue;
			bufferDesc->channelNum = i;
			bufferDesc->buffers[0] = bufferDesc->buffers[1] = nullptr;

			channelDesc->channel = i;
			channelDesc->isInput = ASIOTrue;
			ASIOGetChannelInfo(channelDesc);
		}

		for (long i = 0; i < numOutputChannels; i++, bufferDesc++, channelDesc++)
		{
			bufferDesc->isInput = ASIOFalse;
			bufferDesc->channelNum = i;
			bufferDesc->buffers[0] = bufferDesc->buffers[1] = nullptr;

			channelDesc->channel = i;
			channelDesc->isInput = ASIOFalse;
			ASIOGetChannelInfo(channelDesc);
		}

		ASIOCreateBuffers(bufferInfos, numInputChannels + numOutputChannels, preferredBufferSize, &callbacks);

		delete[] input;
		input = new float *[numInputChannels];
		delete[] output;
		output = new float *[numOutputChannels];
	}

	void start()
	{
		ASIOStart();
	}

	void stop()
	{
		ASIOStop();
	}

	void close()
	{
		delete[] input;
		delete[] output;

		ASIODisposeBuffers();
		delete[] bufferInfos;
		bufferInfos = nullptr;

		delete[] channelInfos;
		channelInfos = nullptr;

		ASIOExit();
		currentDevice = nullptr;
	}

	void setAudioCallback(manAudioCallback callback)
	{
		audioCallback = callback;
	}

	AsioDrivers driverList;
	ASIOCallbacks callbacks;
	ASIODriverInfo driverInfo;
	long numInputChannels;
	long numOutputChannels;
	long minBufferSize;
	long maxBufferSize;
	long preferredBufferSize;
	long granularity;
	bool asioOutputReady;
	ASIOBufferInfo * bufferInfos;
	ASIOChannelInfo * channelInfos;
	ASIOSampleRate sampleRate;
	manAudioCallback audioCallback;
	float ** input;
	float ** output;
};

/* asio callbacks */
void asio_bufferSwitch(long index, ASIOBool directProcess)
{
	asio_bufferSwitchTimeInfo(nullptr, index, directProcess);
}

void asio_sampleRateDidChange(ASIOSampleRate sampleRate)
{
}

long asio_message(long selector, long value, void * message, double * opt)
{
	return 0;
}

ASIOTime * asio_bufferSwitchTimeInfo(ASIOTime * params, long index, ASIOBool directProcess)
{
	if (currentDevice->audioCallback)
	{
		const long bytesPerChannel = currentDevice->preferredBufferSize * sizeof(int32_t);
		for (int i = 0; i < currentDevice->numInputChannels; ++i)
		{
			currentDevice->input[i] = static_cast<float *>(currentDevice->bufferInfos[i].buffers[index]);
			fromInt32ToFloatInPlace(currentDevice->input[i], currentDevice->preferredBufferSize);
		}
		manAudioBuffer inputBuffer = { currentDevice->numInputChannels,
			bytesPerChannel * currentDevice->numInputChannels, currentDevice->input };

		for (size_t i = 0; i < currentDevice->numOutputChannels; ++i)
		{
			currentDevice->output[i] = static_cast<float *>(currentDevice->bufferInfos[i + currentDevice->numInputChannels].buffers[index]);
		}
		manAudioBuffer outputBuffer = { currentDevice->numOutputChannels,
			bytesPerChannel * currentDevice->numOutputChannels, currentDevice->output };

		currentDevice->audioCallback(inputBuffer, outputBuffer);

		for (size_t i = 0; i < currentDevice->numOutputChannels; ++i)
		{
			ASIOConvertSamples().float32toInt32inPlace(currentDevice->output[i], currentDevice->preferredBufferSize);
		}
	}

	if (currentDevice->asioOutputReady)
	{
		ASIOOutputReady();
	}

	return params;
}

manAsioDevice::manAsioDevice()
{
	_private = new manAsioDevicePrivate();
}

manAsioDevice::~manAsioDevice()
{
	delete _private;
}

void manAsioDevice::create(const std::string& name)
{
	if (_private->create(name))
	{
		_private->init();
	}
}

void manAsioDevice::start()
{
	_private->start();
}

void manAsioDevice::stop()
{
	_private->stop();
}

void manAsioDevice::setAudioCallback(manAudioCallback callback)
{
	_private->setAudioCallback(callback);
}

float manAsioDevice::sampleRate()
{
	return static_cast<float>(_private->sampleRate);
}
