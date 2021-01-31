// © 2noodles llc
// minh@2noodles.com


#include "HallSensor.h"
#include "shunt.h"

HallSensor::HallSensor()
{
    //classType inherit from base
    strcpy(classType, "HallSensor");
   
    count = 0;
    lastRead = 0;
    sensorVal = 0;
}

HallSensor::HallSensor(char *in_name, uint8_t in_pin) : Device(), SensorWaac(), Storable()
{
    //deviceID is automatically set my deviceDeleGate

    strcpy(deviceName,in_name);
    //classType inherit from base
    strcpy(classType, "HallSensor");
    
    pin = in_pin;
    // set internal pullup
    pinMode(pin, INPUT_PULLUP);
   
    count = 0;
    lastRead = 0;
    sensorVal = 0;
}


HallSensor::~HallSensor() {
    //cleaup - get rid of interrupt
    detachInterrupt(pin);
}


void HallSensor::setup(void (*ISR_callback)(void), int value) {
    attachInterrupt(digitalPinToInterrupt(pin), ISR_callback, value);
}



const char * HallSensor::read() {
    
    output[70];
    
    //float temp = getNewValue();
    //need the value immidiately since the value is time dependent
    dtostrf(sensorVal, 1, 1, output);
    /*  don't include unit. do it separate
    if(strcmp(unit,"") != 0) {
        strcat(output,unit);
    }
    */
    //printf("read output %s\n", output);
    return output;
    
}

void HallSensor::rpm ()                                   // This is the function that the interupt calls via getRPM()
{
    //gotta be static or crash
    count++;                              // This function measures the rising and falling edge of the hall effect sensors signal
}

float HallSensor::getNewValue() {
//need to make a generic hall sensor- let the math be handled with the shunting yard
     if (millis() - lastRead >=1000) //read interrupt count every second
    {
      lastRead = millis();
    
      noInterrupts(); // disable interrupts for correct calculation
 
      sensorValue[10]={};
      char buffer[10];

      strcpy(sensorValue, itoa(count, buffer, 10));
      if(strcmp(formula,"") != 0) {
        searchAndReplace(var, sensorValue); // replaced 'x' with count
      }

        float temp = 0.0;
        if(strcmp(newFormula,"") != 0) { // newFormula after 'x' is replaced
            temp = shuntThis(newFormula);
            //temp = 5.0;
        }

      count = 0; // reset for next calc

      return temp; // liters/minute

      interrupts(); //start interrupt back up
   
    }
}

void HallSensor::loop() {
     float temp = getNewValue();
    sensorVal = temp;
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

void HallSensor::serialize(JsonObject& doc)
{
    // First call father serialization
    Device::serialize(doc);
    
    SensorWaac::serialize(doc);
}

void HallSensor::deserialize(
    JsonObject& doc)
{
   // First call father deserialization
    Device::deserialize(doc);
    
    SensorWaac::deserialize(doc);

    // set internal pullup
    pinMode(pin, INPUT_PULLUP);
}

