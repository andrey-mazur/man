#include "manAsioDevice.h"

#include <asiosdk/common/asiosys.h>
#include <asiosdk/common/asio.h>
#include <asiosdk/host/asiodrivers.h>


/* asio callbacks declarations */
void asio_bufferSwitch(long index, ASIOBool directProcess);
void asio_sampleRateDidChange(ASIOSampleRate sampleRate);
long asio_message(long selector, long value, void * message, double * opt);
ASIOTime * asio_bufferSwitchTimeInfo(ASIOTime * params, long index, ASIOBool directProcess);

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
		ASIOBufferInfo * info = bufferInfos;
		for (long i = 0; i < numInputChannels; i++, info++)
		{
			info->isInput = ASIOTrue;
			info->channelNum = i;
			info->buffers[0] = info->buffers[1] = nullptr;
		}

		for (long i = 0; i < numOutputChannels; i++, info++)
		{
			info->isInput = ASIOFalse;
			info->channelNum = i;
			info->buffers[0] = info->buffers[1] = nullptr;
		}

		ASIOCreateBuffers(bufferInfos, numInputChannels + numOutputChannels, preferredBufferSize, &callbacks);

		delete[] input;
		input = new void *[numInputChannels];
		delete[] output;
		output = new void *[numOutputChannels];
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
	ASIOSampleRate sampleRate;
	manAudioCallback audioCallback;
	void ** input;
	void ** output;
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
		for (int i = 0; i < currentDevice->numInputChannels; ++i)
		{
			currentDevice->input[i] = currentDevice->bufferInfos[i].buffers[index];
		}
		for (int i = 0; i < currentDevice->numInputChannels; ++i)
		{
			currentDevice->output[i] = currentDevice->bufferInfos[i + currentDevice->numInputChannels].buffers[index];
		}
		currentDevice->audioCallback(currentDevice->input, currentDevice->numInputChannels,
			currentDevice->output, currentDevice->numOutputChannels);
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
	currentDevice = nullptr;
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

long manAsioDevice::bufferSize()
{
	return _private->preferredBufferSize;
}
