#ifndef DeviceDelegate_h
#define DeviceDelegate_h

#include "Arduino.h"
#include "../Device/Device.h"

class DeviceDelegate {
   
    public:
        DeviceDelegate();
        void addDevice(Device *in_device);
        void removeDevice(int in_id);
        Device * findDevice(int in_id);
        void loopDevices();
        uint8_t getDeviceCount();
    
        Device *devices[10];
        Device *currentDevice();
    
        uint8_t numberOfDevices;
        int counter;
    private:
        long previousMillis;
        long interval;
};
extern DeviceDelegate deviceDelegate;
#endif

