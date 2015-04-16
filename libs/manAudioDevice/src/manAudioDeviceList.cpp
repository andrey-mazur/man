#include <manAudioDevice/manAudioDeviceList.h>

#ifdef _WIN32
#include "pc/manAsioDevice.h"
#endif

manAudioDeviceList::manAudioDeviceList()
{
    manAsioDevice device;
}
