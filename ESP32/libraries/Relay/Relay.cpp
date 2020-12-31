/*
  Relay
*/

// © 2noodles llc
// minh@2noodles.com


#include "Relay.h"

Relay::Relay() : Device()
{
    scheduler = Scheduler();
    invert = false;
    dependentDeviceObject = NULL;
}


Relay::Relay(char *in_name, int in_pin, int in_dependent_device_id) : Device()
{
    // Device() call super to init vars
    dependentDeviceId = in_dependent_device_id;
    //find dependent device once - not all the time
    setDependentDevice(dependentDeviceId);
    
    
    //deviceID is automatically set my deviceDeleGate

    strcpy(deviceName,in_name);
    //classType inherit from base
    strcpy(classType,"Relay");

    pin = in_pin;
    pinMode(pin, OUTPUT);
    digitalWrite(pin,LOW); // weird where pin gets high on startup
                           // devices need pullup or pulldown for desired output
    
    scheduler = Scheduler();
    invert = false;
}


void Relay::loop()
{
    // If timer is not suspended then check scheduler and dependent devices.
    if(suspendTime == false) {
        // Check if there is an event in progress now.
        if (false == scheduler.areScheduledEvents()) {
            
            // if there's a dependent device
            if(NULL != dependentDeviceObject) {
                //find dependent device - call DeviceDelegate
                if(true == dependentDeviceObject->getDeviceState()) {
                    switchOn();
                } else {
                    switchOff();
                }
            }
        }
        else if (true == scheduler.isEventInProgress())
        {
            // if there's a dependent device
            if(NULL != dependentDeviceObject) { 
                // find dependent device - call DeviceDelegate
                if(true == dependentDeviceObject->getDeviceState()) {
                    switchOn();
                } else {
                    switchOff();
                }
            }
            else {
                switchOn();
            }
        }
        else {
            switchOff();
        }
    }// end suspend time
}



void Relay::setEvent(char *in_string)
{
    scheduler.setEvent(in_string);
}


void Relay::getEvent(char *string) {
    scheduler.getEvent(string);
}

int Relay::getDependentDevice() {
    return dependentDeviceId;
    
}

void Relay::setDependentDevice(int id) {
    dependentDeviceId = id;
    if(dependentDeviceId > 0) {
        dependentDeviceObject = deviceDelegate.findDevice(dependentDeviceId);
    } else {
        dependentDeviceObject = NULL;
    }

    
}


void Relay::switchOn()
{
    // address is defined in the device cpp file
    //Serial.println("switching on");
    if (false == deviceState)
    {              
        if(invert) {
            digitalWrite(pin,LOW);
        } else {
            digitalWrite(pin,HIGH);
        }
        
        deviceState = true;
    }
}

void Relay::switchOff()
{
    // address is defined in the device cpp file
    //Serial.println("switching off");
    if (true == deviceState)
    {
        if(invert) {
            digitalWrite(pin,HIGH);
        } else {
            digitalWrite(pin,LOW);
        }

        deviceState = false;
    }
}

void Relay::toggleState()
{
   //Serial.println("toggle called");
    if (deviceState) {
        switchOff();
        //Serial.println("toggle off");
    } else {
        switchOn();
        //Serial.println("toggle on");
    }
}

boolean Relay::getInvert() {
    return invert;
}

void Relay::setInvert(boolean state) {
    invert = state;
}

void Relay::serialize(JsonObject& doc)
{
    // First call father serialization
    Device::serialize(doc);
    
    doc["invert"] = invert;
    
    scheduler.serialize(doc);
}

void Relay::deserialize(
    JsonObject& doc)
{
   // First call father deserialization
    Device::deserialize(doc);
    
    invert = doc["invert"];
    
    scheduler.deserialize(doc);
    
    pinMode(pin, OUTPUT);
}
