#pragma once
#include <boost/function.hpp>

enum SampleFormat
{
	SampleFormat_Unknown,
	SampleFormat_Int16,
	SampleFormat_Int32,
	SampleFormat_Float32
};
typedef enum SampleFormat SampleFormat;

struct manAudioBuffer
{
	size_t numChannels;
	size_t numLengthInBytes;
	void ** data;
};
typedef struct manAudioBuffer manAudioBuffer;

struct manAudioBufferList
{	
	int numBuffers;
	manAudioBuffer * buffer;
};
typedef struct manAudioBufferList manAudioBufferList;

typedef boost::function<void(const manAudioBuffer& inputBuffer, manAudioBuffer& outputBuffer)> manAudioCallback;

class manAudioDevice
{
public:
	virtual ~manAudioDevice();

	virtual void start() = 0;
	virtual void stop() = 0;
	virtual void setAudioCallback(manAudioCallback) = 0;
	virtual float sampleRate() = 0;
	virtual SampleFormat sampleFormat() = 0;
};
