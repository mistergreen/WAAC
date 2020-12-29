// © 2noodles llc
// minh@2noodles.com

#include "OneWireSensor.h"
//#include <dtostrf.h> // dosn't need this helper
#include <OneWire.h>
#include <SensorWaac.h>

OneWireSensor::OneWireSensor() : Device(), SensorWaac()
{
    strcpy(temperature, "000");
}

OneWireSensor::OneWireSensor(char *in_name, uint8_t in_pin, float in_min, float in_max) : Device(), SensorWaac()
{
    min = in_min;
    max = in_max;
    pin = in_pin;
    
    //Onewire instance - doesn't let me do automatic instance for some reason
    ds = new OneWire(pin);
    
    strcpy(deviceName,in_name);
    strcpy(classType,"OneWireSensor");
    
    fahrenheit = true;
    strcpy(temperature, "000");
    strcpy(conditionType,"and");
}

OneWireSensor::~OneWireSensor() {
    delete ds;
    ds = NULL;
}

float OneWireSensor::getNewValue() {
       
    byte data[12];
    byte addr[8];
    
    if ( !ds->search(addr)) {
        //no more sensors on chain, reset search
        Serial.println(" No more addresses.");
        Serial.println();
        ds->reset_search();
        delay(250);
        return -1000;
    }
    
    if ( OneWire::crc8( addr, 7) != addr[7]) {
        Serial.println("CRC is not valid!");
        return -1000;
    }
    
    if ( addr[0] != 0x10 && addr[0] != 0x28) {
        Serial.print("Device is not recognized");
        return -1000;
    }
    
    ds->reset();
    ds->select(addr);
    ds->write(0x44,1); // start conversion, with parasite power on at the end

    byte present = ds->reset();
    ds->select(addr);
    ds->write(0xBE); // Read Scratchpad
    
    
    for (int i = 0; i < 9; i++) { // we need 9 bytes
        data[i] = ds->read();
    }
    
    ds->reset_search();
    
    byte MSB = data[1];
    byte LSB = data[0];
    
    float tempRead = ((MSB << 8) | LSB); //using two's compliment
    float TemperatureSum = tempRead / 16;
    
    if(fahrenheit) {
        return TemperatureSum*9/5+32;
    } else {
        return TemperatureSum;
    }
}


const char * OneWireSensor::read() {

    float temp = getNewValue();
    
    dtostrf(temp, 1, 1, temperature);
    
    if(fahrenheit) {
        strcat(temperature, "%C2%B0F");
    } else {
        strcat(temperature, "%C2%B0C");
    }
    
    return temperature;
  
}

void OneWireSensor::loop() {
     float temp = getNewValue();
  
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

void OneWireSensor::setF(bool in_state) {
    fahrenheit = in_state;
}

bool OneWireSensor::getF() {
    return fahrenheit;
}

void OneWireSensor::serialize(JsonObject& doc)
{
    // First call father serialization
    Device::serialize(doc);
    SensorWaac::serialize(doc);
    
    doc["fahrenheit"] = fahrenheit;
}

void OneWireSensor::deserialize(
    JsonObject& doc)
{
   // First call father deserialization
    Device::deserialize(doc);
    SensorWaac::deserialize(doc);
    
    //Onewire instance
    ds = new OneWire(pin);
    
    fahrenheit = doc["fahrenheit"];
}

