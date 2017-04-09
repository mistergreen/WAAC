/*
  Relay
*/

// © 2noodles llc
// minh@2noodles.com

#include "ArduinoRelay.h"
#include "DeviceDelegate.h"
#include <TimeLib.h>

ArduinoRelay::ArduinoRelay(char *in_name, int in_pin, int in_dependent_device_id) : Relay(in_name, in_pin, in_dependent_device_id)
{
    // Device() call super to init vars
    dependentDeviceId = in_dependent_device_id;
    //find dependent device once - not all the time
    setDependentDevice(dependentDeviceId);
    
    
    //deviceID is automatically set my deviceDeleGate

    strcpy(deviceName,in_name);
    //classType inherit from base
    strcpy(classType,"ArduinoRelay");
    
    isDay = false; // isDay is the day an event is to occur
    timedIndexCounter = 0;
    onceFlag = false;
    
}


void ArduinoRelay::switchOn()
{
    if(invert) {
        Serial.print("pin=");
        Serial.print(pin);
        Serial.print("&value=");
        Serial.println("0");
    } else {
        Serial.print("pin=");
        Serial.print(pin);
        Serial.print("&value=");
        Serial.println("1");
    }


    deviceState = true;
}

void ArduinoRelay::switchOff()
{
  
    if(invert) {
        Serial.print("pin=");
        Serial.print(pin);
        Serial.print("&value=");
        Serial.println("1");
    } else {
        Serial.print("pin=");
        Serial.print(pin);
        Serial.print("&value=");
        Serial.println("0");
    }

   
    deviceState = false;
}

void ArduinoRelay::toggleState()
{
  
    if (deviceState) {
        switchOff();
       
    } else {
        switchOn();
       
    }
}


