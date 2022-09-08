// Andrea Villa
// villa.andrea@gmail.com

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

    button.begin(in_pin);
}

const char * InputButton::read() {
    
    output[5];
    
    bool temp = button.isPressed();
    
    if(temp == false) {
        strcpy(output, "LOW");
    } else {
        strcpy(output, "HIGH");
    }
   
    return output;
}

void InputButton::loop() {
    button.loop();
}

void InputButton::serialize(JsonObject& doc)
{
    // First call father serialization
    Device::serialize(doc);
}

void InputButton::deserialize(
    JsonObject& doc)
{
   // First call father deserialization
    Device::deserialize(doc);

    button.begin(pin);
}
