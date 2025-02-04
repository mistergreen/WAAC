// © 2noodles llc
// minh@2noodles.com

#ifndef DeviceDelegate_h
#define DeviceDelegate_h

#include "Arduino.h"
#include "Device.h"

class DeviceDelegate {
   
    public:
        DeviceDelegate();
        void addDevice(Device *in_device);
        // Add a device with a specific id. Used when adding devices from storage.
        void addDevice(Device *in_device, int in_id);
        
        void removeDevice(int in_id);
        Device * findDevice(int in_id);
        void loopDevices();
        uint8_t getDeviceCount();
        Device *currentDevice();

        // The maximum number of allowed devices.
        static const uint8_t sMAX_NUM_DEVICES = 10;
        Device *devices[sMAX_NUM_DEVICES];
    private:

        uint8_t numberOfDevices;
        int counter;

        long previousMillis;
        long interval;
};

extern DeviceDelegate deviceDelegate;
#endif

