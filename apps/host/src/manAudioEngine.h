#pragma once

class manAudioDevice;
class manAudioEngine
{
public:
	explicit manAudioEngine();

    void process(void ** input, long numInputChannels,
		void ** output, long numOutputChannels);
	
	template <typename T>
	void sinWave(void ** input, long numInputChannels,
		void ** output, long numOutputChannels);

private:
	manAudioDevice * _device;
};
