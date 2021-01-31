/*
  AdaFruitAdaFruitPWM8
*/

// © 2noodles llc
// minh@2noodles.com

#define UNSET -1

#include <Arduino.h>
#include "AdaFruitPWM8.h"
#include "PCAhelper.h"
#include <Wire.h>
#include <DeviceDelegate.h>
#include <TimeLib.h>

AdaFruitPWM8::AdaFruitPWM8() : PWM4()
{
    Serial.println("Creating AdaFruitPWM8");

    //classType inherit from base
    strcpy(classType,"AdaFruitPWM8");  

    if(!PCAhelper::isSet) {
        PCAhelper::init();
    }
}


AdaFruitPWM8::AdaFruitPWM8(char *in_name, int in_dependent_device_id, int insda, int inscl) : 
    PWM4(in_name, in_dependent_device_id, 8)
{
    Serial.println("Creating AdaFruitPWM8");

    //classType inherit from base
    strcpy(classType,"AdaFruitPWM8");

    if(!PCAhelper::isSet) {
        PCAhelper::init();
    }

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


void AdaFruitPWM8::setPins(int red, int green, int blue, int white, int aux1, int aux2, int aux3, int aux4) {
    //turn off any previous pin
    //switchOff();
    
    color[0].pin = red;
    color[1].pin = green;
    color[2].pin = blue;
    color[3].pin = white;
    color[4].pin = aux1;
    color[5].pin = aux2;
    color[6].pin = aux3;
    color[7].pin = aux4;
    //Serial.println("pins saved");
   
}


void AdaFruitPWM8::getPins(int *inArray) {
    
     for(int i=0; i < NUM_CHANNELS; i++) {
         inArray[i] = color[i].pin;
     }
    
}


void AdaFruitPWM8::setPWMs(int in_red, int in_green, int in_blue, int in_white, int in_aux1, int in_aux2, int in_aux3, int in_aux4)
{
    int tempColor[8] = {in_red, in_green, in_blue, in_white, in_aux1, in_aux2, in_aux3, in_aux4};
    
    for(uint8_t k=0; k < NUM_CHANNELS; k++) {
        if(color[k].pin > UNSET) {
            PCAhelper::pwm.setPin(color[k].pin, tempColor[k], false);
            //Serial.println(color[k].pin);
            //Serial.println(tempColor[k]);
            //Serial.println("------");
        }
    }
    
    //Serial.println("set pwms");
    //Serial.println(in_red);
}

void AdaFruitPWM8::setI2C(int insda, int inscl) {
    SDA = insda;
    SCL = inscl;
#ifdef ESP8266
        Wire.pins(SDA, SCL);
#elif ESP32
    //setup I2C

    if(SDA > UNSET)
        Wire.begin(SDA, SCL);     // no Wire.pins(1,2) for esp32
#endif

    
}

void AdaFruitPWM8::getI2C(int *inArray) {
    inArray[0] = SDA;
    inArray[1] = SCL;
    
}

void AdaFruitPWM8::setPin(uint8_t channel, uint16_t color)
{
    PCAhelper::pwm.setPin(channel, color, false);
}


void AdaFruitPWM8::serialize(JsonObject& doc)
{
    // First call father serialization
    Device::serialize(doc);
    EventHandler::serialize(doc);

    char event[4*20];
    // clean the buffer
    memset(event, 0, sizeof(event));

    getEventColors(event);
    doc["eventColors"] = event;

    doc["pin0"] = color[0].pin;
    doc["pin1"] = color[1].pin;
    doc["pin2"] = color[2].pin;
    doc["pin3"] = color[3].pin;
    doc["pin4"] = color[4].pin;
    doc["pin5"] = color[5].pin;
    doc["pin6"] = color[6].pin;
    doc["pin7"] = color[7].pin;

    doc["SDA"] = SDA;
    doc["SCL"] = SCL;
}

void AdaFruitPWM8::deserialize(
    JsonObject& doc)
{
   // First call father deserialization
    Device::deserialize(doc);
    EventHandler::deserialize(doc);

    char event[4*20];
    // clean the buffer
    memset(event, 0, sizeof(event));
    strcpy (event, doc["eventColors"]);
    
    setEventColors(event);

    setPins(doc["pin0"],
            doc["pin1"],
            doc["pin2"],
            doc["pin3"],
            doc["pin4"],
            doc["pin5"],
            doc["pin6"],
            doc["pin7"]);

    SDA = doc["SDA"];
    SCL = doc["SCL"];
}