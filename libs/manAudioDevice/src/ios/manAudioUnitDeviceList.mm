#import "manAudioUnitDeviceList.h"

#import "manAudioUnitDevice.h"
#import <AudioUnit/AudioUnit.h>


std::vector<std::string> manAudioUnitDeviceList::enumerate()
{
}

manAudioDevice * manAudioUnitDeviceList::create(const std::string& name)
{
    manAudioUnitDevice * pAudioUnitDevice = new manAudioUnitDevice();
    pAudioUnitDevice->create(name);
    return pAudioUnitDevice;
}
