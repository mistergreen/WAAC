// © 2noodles llc
// minh@2noodles.com

#include "DeviceDelegate.h"


//don't need contructor in static class


DeviceDelegate::DeviceDelegate() {
    numberOfDevices = 0;
    previousMillis = 0;
    interval = 1000;
    counter = 1;
    
}


void DeviceDelegate::addDevice(Device *in_device) {
    devices[numberOfDevices] = in_device;
    devices[numberOfDevices]->setDeviceId(counter);
    numberOfDevices++;
    counter++;
}

void DeviceDelegate::removeDevice(int in_id) {
    //slice array
    //find device base on id
    // **********realy need to test this out!!!!
    uint8_t i;
    for(i=0; i < numberOfDevices; i++) {
        if (devices[i]->deviceId == in_id) {
            delete devices[i];
            devices[i] = NULL;
            break;
        }
        
    }
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

Device * DeviceDelegate::findDevice(int in_id) {
    for(uint8_t i=0; i < numberOfDevices; i++) {
        if (devices[i]->deviceId == in_id) {
            return devices[i];
        }
        
    }
   
}

Device * DeviceDelegate::currentDevice() {
    return devices[numberOfDevices-1];
}



void DeviceDelegate::loopDevices() {
    //calls loop in all devices every second
    /*
    unsigned long currentMillis = millis();
    if(currentMillis - previousMillis > interval) {
        previousMillis = currentMillis;   
        for(uint8_t i=0; i < numberOfDevices; i++) {
            devices[i]->loop();
        }
    }
     */
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





