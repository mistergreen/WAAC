/*
  RelayMCP
*/

// © 2noodles llc
// minh@2noodles.com

#define UNSET -1

#include "RelayMCP.h"
#include "DeviceDelegate.h"
#include "MCPhelper.h"
#include "Relay.h"
#include <Wire.h>


RelayMCP::RelayMCP() : Relay()
{
    //classType inherit from base
    strcpy(classType,"RelayMCP");

    //pin config
    // GPA0 = #21 alias 0
    // GPB0 = #1 alias 8
    // GPB1 = #2 alias 9
    // Looks like you can only begin() once or it'll blow up
    // maybe do it once in a static class
    if(!MCPhelper::isSet) {
        MCPhelper::init();
    }
}

RelayMCP::RelayMCP(char *in_name, int in_pin, int in_dependent_device_id)
{
    Relay(in_name, in_pin, in_dependent_device_id);

    strcpy(deviceName,in_name);
    //classType inherit from base
    strcpy(classType,"RelayMCP");

    //pin config
    // GPA0 = #21 alias 0
    // GPB0 = #1 alias 8
    // GPB1 = #2 alias 9
    // Looks like you can only begin() once or it'll blow up
    // maybe do it once in a static class
    if(!MCPhelper::isSet) {
        MCPhelper::init();
    }

    MCPhelper::mcp.pinMode(pin, OUTPUT);
    MCPhelper::mcp.pullUp(pin, LOW); // in case it was set HIGH previously
}

void RelayMCP::setI2C(int insda, int inscl) 
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

void RelayMCP::getI2C(int *inArray) {
    inArray[0] = SDA;
    inArray[1] = SCL;
    
}

void RelayMCP::switchOn()
{
    // address is defined in the device cpp file
    //Serial.println("switching on");
    if(invert) {
        //LOW
        MCPhelper::mcp.digitalWrite(pin, LOW);
    } else {
        //HIGH
        MCPhelper::mcp.digitalWrite(pin, HIGH);
    }

   
    deviceState = true;
}

void RelayMCP::switchOff()
{
    // address is defined in the device cpp file
    //Serial.println("switching off");

    if(invert) {
        MCPhelper::mcp.digitalWrite(pin, HIGH);
    } else {
        MCPhelper::mcp.digitalWrite(pin, LOW);
    }

    deviceState = false;
}

void RelayMCP::serialize(JsonObject& doc)
{
    // First call father serialization
    Device::serialize(doc);
    
    Relay::serialize(doc);
    
    doc["SDA"] = SDA;
    doc["SCL"] = SCL;

}

void RelayMCP::deserialize(
    JsonObject& doc)
{
   // First call father deserialization
    Device::deserialize(doc);
    
    Relay::deserialize(doc);
    
    MCPhelper::mcp.pinMode(pin, OUTPUT);
    MCPhelper::mcp.pullUp(pin, LOW); // in case it was set HIGH previously
    
    setI2C(doc["SDA"], doc["SCL"]);
}

