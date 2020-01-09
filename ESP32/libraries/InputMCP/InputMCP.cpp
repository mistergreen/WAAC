/*
 InputMCP
// © 2noodles llc
// minh@2noodles.com
*/
#define UNSET -1

#include <Arduino.h>
#include "InputMCP.h"
#include "MCPhelper.h"

InputMCP::InputMCP(char *in_name, uint8_t in_pin) : Device(), SensorWaac()
{
    //deviceID is automatically set my deviceDeleGate

    strcpy(deviceName,in_name);
    //classType inherit from base
    strcpy(classType, "InputMCP");
    
    pin = in_pin;

    //pin config
    // GPA0 = #21 alias 0
    // GPB0 = #1 alias 8
    // GPB1 = #2 alias 9
    //
    // Input signal starts HIGH, button must connect from pin to ground and read LOW when pressed
    if(!MCPhelper::isSet) {
        MCPhelper::init();
    }
    
    MCPhelper::mcp.pinMode(pin, INPUT);
    MCPhelper::mcp.pullUp(pin, HIGH);  // turn on a 100K pullup internally

    lastDebounceTime = 0;  // the last time the output pin was toggled
    debounceDelay = 50; 
    lastState = LOW; 
}


const char * InputMCP::read() {
    
    output[5];
    
    bool temp = getNewValue();
    
    if(temp == false) {
        strcpy(output, "LOW");
    } else {
        strcpy(output, "HIGH");
    }
   
    return output;
    
}

bool InputMCP::getNewValue() {

    return MCPhelper::mcp.digitalRead(pin);
    
}

void InputMCP::loop() {
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

void InputMCP::setI2C(int insda, int inscl) 
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

void InputMCP::getI2C(int *inArray) {
    inArray[0] = SDA;
    inArray[1] = SCL;
    
}


