/*
  RelayPCA
*/

// © 2noodles llc
// minh@2noodles.com

#define UNSET -1

#include "RelayPCA.h"
#include "DeviceDelegate.h"
#include "Relay.h"
#include "PCAhelper.h"
#include <TimeLib.h>
#include <Wire.h>


RelayPCA::RelayPCA() : Relay()
{
    if(!PCAhelper::isSet) {
        PCAhelper::init();
    }
}

RelayPCA::RelayPCA(char *in_name, int in_pin, int in_dependent_device_id)
{
     Relay(in_name, in_pin, in_dependent_device_id);

    strcpy(deviceName,in_name);
    //classType inherit from base
    strcpy(classType,"RelayPCA");

    pin = in_pin;

    isDay = false; // isDay is the day an event is to occur
    timedIndexCounter = 0;
    onceFlag = false;

    if(!PCAhelper::isSet) {
        PCAhelper::init();
    }
}

void RelayPCA::setI2C(int insda, int inscl) 
{
    SDA = insda;
    SCL = inscl;

#ifdef ESP8266
    //setup I2C
    Wire.pins(SDA, SCL);
    //Wire.pins(2, 14);   // ESP8266 can use any two pins, such as SDA to #2 and SCL to #14

#elif ESP32
   
    //setup I2C, ESP32 default is pin #21 SDA, 22 SCL
    if(SDA > UNSET)
        Wire.begin(SDA, SCL);     // no Wire.pins(1,2) for esp32
   
#endif

}

void RelayPCA::getI2C(int *inArray) {
    inArray[0] = SDA;
    inArray[1] = SCL;
    
}

void RelayPCA::switchOn()
{
    // address is defined in the device cpp file
    //Serial.println("switching on");
    if(invert) {
        //LOW
        PCAhelper::pwm.setPin(pin, 0, false);
    } else {
        //HIGH
        PCAhelper::pwm.setPin(pin, 4095, false);
    }

   
    deviceState = true;
}

void RelayPCA::switchOff()
{
    // address is defined in the device cpp file
    //Serial.println("switching off");

    if(invert) {
        PCAhelper::pwm.setPin(pin, 4095, false);
    } else {
        PCAhelper::pwm.setPin(pin, 0, false);
    }

    deviceState = false;
}

void RelayPCA::serialize(JsonObject& doc)
{
    // First call father serialization
    Device::serialize(doc);
    
    Relay::serialize(doc);
    
    doc["SDA"] = SDA;
    doc["SCL"] = SCL;

}

void RelayPCA::deserialize(
    JsonObject& doc)
{
   // First call father deserialization
    Device::deserialize(doc);
    
    Relay::deserialize(doc);

    setI2C(doc["SDA"], doc["SCL"]);
}
