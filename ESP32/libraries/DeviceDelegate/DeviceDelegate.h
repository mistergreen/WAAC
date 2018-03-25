// © 2noodles llc
// minh@2noodles.com

#ifndef DeviceDelegate_h
#define DeviceDelegate_h
#define MAXDEVICE   20


#include "Arduino.h"
#include "Device.h"

class DeviceDelegate {
   
    public:
        DeviceDelegate();
        void addDevice(Device *in_device);
        void removeDevice(int in_id);
        Device * findDevice(int in_id);

        void loopDevices();
        uint8_t getDeviceCount();
    
        Device *devices[MAXDEVICE];
        Device *currentDevice();
    
        uint8_t numberOfDevices;
        int counter;
    private:
        long previousMillis;
        long interval;
};
extern DeviceDelegate deviceDelegate;
#endif

