// Andrea Villa
// villa.andrea@gmail.com

#include <DeviceDelegate.h>
#include "WWWsettings.h"
#include "Esp.h"
#include "InputButton.hpp"

InputButton::InputButton()
{
    //classType inherit from base
    strcpy(classType, "InputButton");
}

InputButton::InputButton(char *in_name, uint8_t in_pin) : Device(), Storable()
{
    //deviceID is automatically set my deviceDeleGate

    strcpy(deviceName, in_name);
    //classType inherit from base
    strcpy(classType, "InputButton");

    pin = in_pin;

    button.begin(in_pin);

    memset(&actions[0], 0, 4*sizeof(int));
    memset(&devicesId[0], 0, 4*sizeof(int));

    for (char i = 0; i<4; i++)
    {
        devices[i] = static_cast<Device*>(NULL);
    }
}

void InputButton::setClickAction(int device, int action)
{
    // System actions
    if (device == 255)
    {
        devices[0] = &systemDevice;
        devicesId[0] = device;
        actions[0] = action;
    }
    else
    {
        devices[0] = deviceDelegate.findDevice(device);
        devicesId[0] = device;
        actions[0] = action;
    }
}

void InputButton::setDoubleClickAction(int device, int action)
{
        // System actions
    if (device == 255)
    {
        devices[1] = &systemDevice;
        devicesId[1] = device;
        actions[1] = action;
    }
    else
    {
        devices[1] = deviceDelegate.findDevice(device);
        devicesId[1] = device;
        actions[1] = action;
    }
}

void InputButton::setTripleClickAction(int device, int action)
{
        // System actions
    if (device == 255)
    {
        devices[2] = &systemDevice;
        devicesId[2] = device;
        actions[2] = action;
    }
    else
    {
        devices[2] = deviceDelegate.findDevice(device);
        devicesId[2] = device;
        actions[2] = action;
    }
}

void InputButton::setLongClickAction(int device, int action)
{
        // System actions
    if (device == 255)
    {
        devices[3] = &systemDevice;
        devicesId[3] = device;
        actions[3] = action;
    }
    else
    {
        devices[3] = deviceDelegate.findDevice(device);
        devicesId[3] = device;
        actions[3] = action;
    }
}

int InputButton::getClickDevice()
{
    return devicesId[0];
}

int InputButton::getDoubleClickDevice()
{
    return devicesId[1];
}

int InputButton::getTripleClickDevice()
{
    return devicesId[2];
}

int InputButton::getLongClickDevice()
{
    return devicesId[3];
}

int InputButton::getClickAction()
{
    return actions[0];
}

int InputButton::getDoubleClickAction()
{
    return actions[1];
}

int InputButton::getTripleClickAction()
{
    return actions[2];
}

int InputButton::getLongClickAction()
{
    return actions[3];
}

const char * InputButton::read() {
    
    output[5];
    
    bool temp = button.wasPressed();
    
    if(temp == false) {
        strcpy(output, "LOW");
    } else {
        strcpy(output, "HIGH");
    }
   
    return output;
}

void InputButton::loop() {
    button.loop();

    if (button.wasPressed()) {

        // The selected button event.
        char buttonEvent = 0;

        switch(button.read()) {
            case single_click: 
                Serial.println("single");
                buttonEvent = 0;
                break;
            case double_click: 
                Serial.println("double");
                buttonEvent = 1;
                break;
            case triple_click: 
                Serial.println("triple");
                buttonEvent = 2;
                break;
            case long_click: 
                buttonEvent = 3;
                Serial.println("looong");
                break;
        }
        
        // First check the pointer then call the action
        if (static_cast<Device*> (NULL) != devices[buttonEvent])
            devices[buttonEvent]->callButtonAction(actions[buttonEvent]);
    }
}

void InputButton::serialize(JsonObject& doc)
{
    // First call father serialization
    Device::serialize(doc);

    doc["actions0"] = actions[0];
    doc["actions1"] = actions[1];
    doc["actions2"] = actions[2];
    doc["actions3"] = actions[3];
    doc["device0"] = devicesId[0];
    doc["device1"] = devicesId[1];
    doc["device2"] = devicesId[2];
    doc["device3"] = devicesId[3];
}

void InputButton::deserialize(
    JsonObject& doc)
{
   // First call father deserialization
    Device::deserialize(doc);

    button.begin(pin);

    if (doc.containsKey("actions0"))
        actions[0] = doc["actions0"].as<int>();
    if (doc.containsKey("actions1"))
        actions[1] = doc["actions1"].as<int>();
    if (doc.containsKey("actions2"))
        actions[2] = doc["actions2"].as<int>();
    if (doc.containsKey("actions3"))
        actions[3] = doc["actions3"].as<int>();
    if (doc.containsKey("device0"))
        devicesId[0] = doc["device0"].as<int>();
    if (doc.containsKey("device1"))
        devicesId[1] = doc["device1"].as<int>();
    if (doc.containsKey("device2"))
        devicesId[2] = doc["device2"].as<int>();
    if (doc.containsKey("device3"))
        devicesId[3] = doc["device3"].as<int>();

    for (char i = 0; i<4; i++)
    {
        devices[i] = static_cast<Device*>(NULL);
        
        // First check the pointer then call the action
        if (devicesId[i] > 0)
        {
            if (devicesId[i] == 255)
            {
                devices[i] = &systemDevice;
            }
            else
            {
                devices[i] = deviceDelegate.findDevice(devicesId[i]);

            }
        }
    }
}


void InputButton::SystemDevice::callButtonAction(
    // The action to call
    int actionId)
{
    WWWsettings* wwws = WWWsettings::getinstance();

    // Check the id before parforming any action.
    if (actionId == 0)
    {
        Serial.print("SystemDevice::callButtonAction calling action ");
        Serial.println(actionId);
       
    }
    else if (actionId == 1)
    {
        Serial.print("SystemDevice::callButtonAction calling action ");
        Serial.println(actionId);
        // Reset
        ESP.restart();
    }
}