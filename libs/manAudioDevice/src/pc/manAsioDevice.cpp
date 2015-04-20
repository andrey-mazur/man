#include "manAsioDevice.h"

#include <asiosdk/common/asiosys.h>
#include <asiosdk/common/asio.h>
#include <asiosdk/host/asiodrivers.h>
#include <stdint.h>
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <math.h>

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
		: _callbacks{}
		, _driverInfo{}
		, _numInputChannels(0)
		, _numOutputChannels(0)
		, _minBufferSize(0)
		, _maxBufferSize(0)
		, _preferredBufferSize(0)
		, _granularity(0)
		, _asioOutputReady(false)
		, _bufferInfos(nullptr)
		, _sampleRate{}
	{
		_callbacks.bufferSwitch = asio_bufferSwitch;
		_callbacks.sampleRateDidChange = asio_sampleRateDidChange;
		_callbacks.asioMessage = asio_message;
		_callbacks.bufferSwitchTimeInfo = asio_bufferSwitchTimeInfo;
		_driverInfo.asioVersion = 2;
	}

	~manAsioDevicePrivate()
	{
		stop();
		close();
	}

	bool create(const std::string& name)
	{
		if (_driverList.loadDriver(const_cast<char *>(name.c_str())))
		{
			currentDevice = this;
			return true;
		}

		return false;
	}

	void init()
	{
		ASIOInit(&_driverInfo);

		ASIOGetChannels(&_numInputChannels, &_numOutputChannels);

		ASIOGetBufferSize(&_minBufferSize, &_maxBufferSize, &_preferredBufferSize, &_granularity);

		ASIOGetSampleRate(&_sampleRate);

		_asioOutputReady = ASIOOutputReady() == ASE_OK;

		if (_bufferInfos)
		{
			ASIODisposeBuffers();
			delete[] _bufferInfos;
		}
		_bufferInfos = new ASIOBufferInfo[_numInputChannels + _numOutputChannels];
		ASIOBufferInfo * info = _bufferInfos;
		for (long i = 0; i < _numInputChannels; i++, info++)
		{
			info->isInput = ASIOTrue;
			info->channelNum = i;
			info->buffers[0] = info->buffers[1] = nullptr;
		}

		for (long i = 0; i < _numOutputChannels; i++, info++)
		{
			info->isInput = ASIOFalse;
			info->channelNum = i;
			info->buffers[0] = info->buffers[1] = nullptr;
		}

		ASIOCreateBuffers(_bufferInfos, _numInputChannels + _numOutputChannels, _preferredBufferSize, &_callbacks);
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
		ASIODisposeBuffers();
		delete[] _bufferInfos;
		_bufferInfos = nullptr;

		ASIOExit();
		currentDevice = nullptr;
	}

	AsioDrivers _driverList;
	ASIOCallbacks _callbacks;
	ASIODriverInfo _driverInfo;
	long _numInputChannels;
	long _numOutputChannels;
	long _minBufferSize;
	long _maxBufferSize;
	long _preferredBufferSize;
	long _granularity;
	bool _asioOutputReady;
	ASIOBufferInfo * _bufferInfos;
	ASIOSampleRate _sampleRate;
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
	const double bufferSize = currentDevice->_preferredBufferSize;
	const double freq = 440.0;
	const double d = 2.0 * M_PI * freq / currentDevice->_sampleRate;
	static double sinValue = 0.0;
	const double amplitude = 0.02;
	
	for (long j = 0; j < bufferSize; ++j)
	{
		int32_t value = static_cast<int32_t>(amplitude * std::sin(sinValue) * std::numeric_limits<int32_t>::max());
		
		ASIOBufferInfo * info = currentDevice->_bufferInfos;
		info += currentDevice->_numInputChannels;
		for (long i = 0; i < currentDevice->_numOutputChannels; ++i, ++info)
		{
			int32_t * input = reinterpret_cast<int32_t *>(currentDevice->_bufferInfos[i].buffers[index]);
			input += j;
			
			int32_t * ptr = reinterpret_cast<int32_t *>(info->buffers[index]);
			ptr += j;
			*ptr = value;
			*ptr += *input;
		}
		sinValue += d;
	}

	if (currentDevice->_asioOutputReady)
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
