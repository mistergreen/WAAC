/*
  Relay
*/

// © 2noodles llc
// minh@2noodles.com


#include "Relay.h"

Relay::Relay() : Device(), EventHandler()
{
    //classType inherit from base
    strcpy(classType, "Relay");

    invert = false;
}


Relay::Relay(char *in_name, int in_pin, int in_dependent_device_id) : Device(), EventHandler(in_dependent_device_id)
{
    strcpy(deviceName, in_name);
    //classType inherit from base
    strcpy(classType, "Relay");

    pin = in_pin;
    pinMode(pin, OUTPUT);
    digitalWrite(pin,LOW); // weird where pin gets high on startup
                           // devices need pullup or pulldown for desired output

    invert = false;
}


void Relay::loop()
{
    EventHandler::loop();
}


void Relay::performActionInEvent()
{
    // Change state only if needed, so only once.
    if (false == deviceState)
    {
        switchOn();

        deviceState = true;
    }
}


void Relay::performActionOutEvent()
{
    // Change state only if needed.
    if (true == deviceState)
    {
        switchOff();

        deviceState = false;
    }
}


void Relay::switchOn()
{              
    Serial.println("Relay::switchOn switching on");

    if(invert) {
        digitalWrite(pin,LOW);
    } else {
        digitalWrite(pin,HIGH);
    }
}

void Relay::switchOff()
{
    Serial.println("Relay::switchOff switching off");

    if(invert) {
        digitalWrite(pin,HIGH);
    } else {
        digitalWrite(pin,LOW);
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
    EventHandler::serialize(doc);
    
    doc["invert"] = invert;
}

void Relay::deserialize(
    JsonObject& doc)
{
   // First call father deserialization
    Device::deserialize(doc);
    EventHandler::deserialize(doc);
    
    invert = doc["invert"];
    
    pinMode(pin, OUTPUT);
}
