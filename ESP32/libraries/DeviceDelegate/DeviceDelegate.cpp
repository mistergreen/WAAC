// © 2noodles llc
// minh@2noodles.com

#include <Math.h>
#include "DeviceDelegate.h"


//don't need contructor in static class


DeviceDelegate::DeviceDelegate() {
    numberOfDevices = 0;
    previousMillis = 0;
    interval = 1000;
    counter = 1;
    
}


void DeviceDelegate::addDevice(Device *in_device) {
    // Check to be in the limits.
    if (numberOfDevices < sMAX_NUM_DEVICES)
    {
        devices[numberOfDevices] = in_device;
        devices[numberOfDevices]->setDeviceId(counter);
        numberOfDevices++;
        counter++;
    }
}


void DeviceDelegate::addDevice(Device *in_device, int in_id) {
    // Check to be in the limits and that there is no other device with same id.
    if ((numberOfDevices < sMAX_NUM_DEVICES) && (static_cast<Device*> (NULL) == findDevice(in_id)))
    {
        devices[numberOfDevices] = in_device;
        numberOfDevices++;

        counter = max(counter, (in_id + 1));
    }
}


void DeviceDelegate::removeDevice(int in_id) {
    //slice array
    //find device base on id
    // **********realy need to test this out!!!!
    uint8_t i;

    // Check that the device is present.
    bool found = false;

    for(i=0; i < numberOfDevices; i++) {
        if (devices[i]->deviceId == in_id) {
            delete devices[i];
            devices[i] = NULL;
            found = true;
            break;
        }
        
    }

    // Proceed only if the device id was found.
    if (true == found)
    {
        //move devices after this object up but not if you're the last device
        if(i != (numberOfDevices-1)) {
            uint8_t j;
            for(j = i; j < numberOfDevices; j++) {
                devices[j] = devices[j+1];
            }
            //kill the last one
            delete devices[numberOfDevices-1];
            devices[numberOfDevices-1] = NULL;
            //Serial.println("xxxxx move list up xxxxxxx");
        
        }

        numberOfDevices--;
    }
}

Device * DeviceDelegate::findDevice(int in_id) {
    // The returned pointer.
    Device * retVal = static_cast<Device*> (NULL);

    for(uint8_t i=0; i < numberOfDevices; i++) {
        if (devices[i]->deviceId == in_id) {
            retVal = devices[i];
        }
    }

    return retVal;
}

Device * DeviceDelegate::currentDevice() {
    return devices[numberOfDevices-1];
}



void DeviceDelegate::loopDevices() {
    //call as fast as you can
    for(uint8_t i=0; i < numberOfDevices; i++) {
        devices[i]->loop();
    }
    
}

uint8_t DeviceDelegate::getDeviceCount() {
    return numberOfDevices;
}


//define instance
DeviceDelegate deviceDelegate;
