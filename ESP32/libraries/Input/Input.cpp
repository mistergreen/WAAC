/*
 Input
// © 2noodles llc
// minh@2noodles.com
*/

#include <Arduino.h>
#include "Input.h"

Input::Input()
{
    //classType inherit from base
    strcpy(classType, "Input");

    lastDebounceTime = 0;  // the last time the output pin was toggled
    debounceDelay = 50; 
    lastState = LOW; 
}


Input::Input(char *in_name, uint8_t in_pin) : Device(), SensorWaac(), Storable()
{
    //deviceID is automatically set my deviceDeleGate

    strcpy(deviceName,in_name);
    //classType inherit from base
    strcpy(classType, "Input");
    
    pin = in_pin;

    pinMode(pin, INPUT);

    lastDebounceTime = 0;  // the last time the output pin was toggled
    debounceDelay = 50; 
    lastState = LOW; 
}


const char * Input::read() {
    
    output[5];
    
    bool temp = getNewValue();
    
    if(temp == false) {
        strcpy(output, "LOW");
    } else {
        strcpy(output, "HIGH");
    }
   
    return output;
    
}

bool Input::getNewValue() {

    return digitalRead(pin);
    
}

void Input::loop() {
    bool temp = getNewValue();

    //debounce code - do it with code instead of hardware
    // If the switch changed, due to noise or pressing:
    if(temp != lastState) 
    {
        // reset the debouncing timer
        lastDebounceTime = millis();
    }

    if ((millis() - lastDebounceTime) > debounceDelay) {
        // whatever the reading is at, it has been there for longer than the debounce
        // delay, so take it as the actual current state

         if (temp != deviceState) 
            deviceState = temp; // set deviceState 

    }

    // save the reading. Next time through the loop, it'll be the lastState:
    lastState = temp;


}

void Input::serialize(JsonObject& doc)
{
    // First call father serialization
    Device::serialize(doc);
    
    SensorWaac::serialize(doc);
}

void Input::deserialize(
    JsonObject& doc)
{
   // First call father deserialization
    Device::deserialize(doc);
    
    SensorWaac::deserialize(doc);

    pinMode(pin, INPUT);
}
