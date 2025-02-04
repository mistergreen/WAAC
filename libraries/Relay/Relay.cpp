/*
  Relay
*/

// © 2noodles llc
// minh@2noodles.com


#include "Relay.h"

const char* Relay::sNAME_BUTTON_ACTIONS[] = {"Toggle State", "Set to auto"};

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
    }
}


void Relay::performActionOutEvent()
{
    // Change state only if needed.
    if (true == deviceState)
    {
        switchOff();
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

    deviceState = true;
}

void Relay::switchOff()
{
    Serial.println("Relay::switchOff switching off");

    if(invert) {
        digitalWrite(pin,HIGH);
    } else {
        digitalWrite(pin,LOW);
    }

    deviceState = false;
}

void Relay::toggleState()
{
    EventHandler::setSuspendTime(true);
    //Serial.println("toggle action");
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

void Relay::deserialize(JsonObject& doc)
{
   // First call father deserialization
    Device::deserialize(doc);
    EventHandler::deserialize(doc);
    if (doc.containsKey("invert"))
        invert = doc["invert"].as<bool>();
    
    pinMode(pin, OUTPUT);

    switchOff();
    deviceState = false;
}


void Relay::setSuspendTime(
    bool in_suspend)
{
    EventHandler::setSuspendTime(in_suspend);
}

bool Relay::getSuspendTime()
{
    bool suspendTime = EventHandler::getSuspendTime();
    return suspendTime;
}

int Relay::getNumButtonActions()
{
    return sNUM_BUTTON_ACTIONS;
}


const char* Relay::getButtonActionName(
    int actionId)
{
    // The returned value.
    char* retVal = "";

    // Check the id before parforming any action.
    if (actionId < sNUM_BUTTON_ACTIONS)
    {
        retVal = (char *) sNAME_BUTTON_ACTIONS[actionId];
    }

    return retVal;
}

void Relay::callButtonAction(
    int actionId)
{
    // Check the id before parforming any action.
    if (actionId == 0)
    {
        Serial.print("Device::callButtonAction calling action ");
        Serial.println(actionId);
        toggleState();
    }
    else if (actionId == 1)
    {
        Serial.print("Device::callButtonAction calling action ");
        Serial.println(actionId);
        EventHandler::setSuspendTime(false);
    }
}