/*
  Analog
*/
#include "Analog.h"
#include "shunt.h"

    
Analog::Analog() : Device(), SensorWaac()
{
    //classType inherit from base
    strcpy(classType, "Analog");
}


Analog::Analog(char *in_name, uint8_t in_pin) : Device(), SensorWaac()
{
    //deviceID is automatically set my deviceDeleGate

    strcpy(deviceName,in_name);
    //classType inherit from base
    strcpy(classType, "Analog");
    
    pin = in_pin;

    pinMode(pin, INPUT);

    
}


const char * Analog::read() {
    
    output[70];
    
    float temp = getNewValue();
    
    dtostrf(temp, 1, 1, output);
   
    return output;
    
}

float Analog::getNewValue() {

    char buffer[10];
    strcpy(sensorValue, itoa(analogRead(pin),buffer,10));
    
    if(strcmp(formula,"") != 0) {
        searchAndReplace(var, sensorValue);
    }
    
    //Serial.println("new formula ");
    //Serial.println(newFormula);
    float temp = 0.0;
    if(strcmp(newFormula,"") != 0) {
        temp = shuntThis(newFormula);
    }

    return temp;
    
}

void Analog::loop() {
    if ((millis() - lastRead) > 1000UL) //read interrupt count every second
    {
        float temp = getNewValue();
        lastRead = millis();
        if(strcmp(conditionType, "and") == 0) {
            //prevents rapid switching by offsetting the values
            if(temp < lowerTarget-1) {
                //lower target
                deviceState = false; //defined in Device not sensor
                
            } else if(temp > higherTarget+1) {
                //high target
                deviceState = false;
            } else if(temp <= higherTarget || temp >= lowerTarget) {
                //within target
                deviceState = true;
            }
        } else if(strcmp(conditionType, "greater") == 0) {
            if(temp < lowerTarget-1) {
                //lower target
                deviceState = false;
                
            } else if(temp > higherTarget) {
                //high target
                deviceState = true;
            }
        } else if(strcmp(conditionType, "lesser") == 0) {
            if(temp < lowerTarget) {
                //lower target
                deviceState = true;
                
            } else if(temp > higherTarget+1) {
                //high target
                deviceState = false;
            }
        } else if(strcmp(conditionType, "not") == 0) {
            if(temp < lowerTarget-1) {
                //lower target
                deviceState = true;
                
            } else if(temp > higherTarget+1) {
                //high target
                deviceState = true;
            } else if(temp <= higherTarget || temp >= lowerTarget) {
                deviceState = false;
            }
        }

    }

    
}


void Analog::serialize(JsonObject& doc)
{
    Device::serialize(doc);
    SensorWaac::serialize(doc);
}


void Analog::deserialize(
    JsonObject& doc)
{
    Device::deserialize(doc);
    SensorWaac::deserialize(doc);

    pinMode(pin, INPUT);
    
}
