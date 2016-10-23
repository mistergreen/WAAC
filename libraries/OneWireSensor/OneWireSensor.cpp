/*

*/

#include "OneWireSensor.h"
#include <dtostrf.h>

OneWireSensor::OneWireSensor(char *in_name, uint8_t in_pin, float in_min, float in_max) : Device(), Sensor()
{
    min = in_min;
    max = in_max;
    pin = in_pin;
    
    //Onewire instance
    this->ds = new OneWire(pin);
    
    strcpy(deviceName,in_name);
    strcpy(classType,"OneWireSensor");
    
    fahrenheit = true;
    strcpy(temperature, "000");
    strcpy(conditionType,"and");
}

OneWireSensor::~OneWireSensor() {
    delete this->ds;
    this->ds = NULL;
}

float OneWireSensor::getTemp() {
       
    byte data[12];
    byte addr[8];
    
    if ( !this->ds->search(addr)) {
        //no more sensors on chain, reset search
        this->ds->reset_search();
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
    
    this->ds->reset();
    this->ds->select(addr);
    this->ds->write(0x44,1); // start conversion, with parasite power on at the end
    
    byte present = this->ds->reset();
    this->ds->select(addr);
    this->ds->write(0xBE); // Read Scratchpad
    
    
    for (int i = 0; i < 9; i++) { // we need 9 bytes
        data[i] = this->ds->read();
    }
    
    this->ds->reset_search();
    
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


char * OneWireSensor::read() {

    float temp = getTemp();
    
    dtostrf(temp, 1, 1, temperature);
    
    if(fahrenheit) {
        strcat(temperature, "%C2%B0F");
    } else {
        strcat(temperature, "%C2%B0C");
    }
    
    return temperature;
  
}

void OneWireSensor::loop() {
    
    float temp = getTemp();
    if(strcmp(conditionType, "and") == 0) {
        if(temp < lowerTarget) {
            //lower target
            deviceState = false;

        } else if(temp > higherTarget) {
            //high target
            deviceState = false;
        } else {
            //within target
            deviceState = true;
        }
    } else if(strcmp(conditionType, "greater") == 0) {
        if(temp < lowerTarget) {
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
            
        } else if(temp > higherTarget) {
            //high target
            deviceState = false;
        }
    } else if(strcmp(conditionType, "not") == 0) {
        if(temp < lowerTarget) {
            //lower target
            deviceState = true;
            
        } else if(temp > higherTarget) {
            //high target
            deviceState = true;
        } else {
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



