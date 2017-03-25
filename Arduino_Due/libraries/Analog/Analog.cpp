/*
  Analog
*/
// © 2noodles llc
// minh@2noodles.com

#include "Analog.h"
#include <dtostrf.h>


Analog::Analog(char *in_name, uint8_t in_pin) : Device(), Sensor()
{
    //deviceID is automatically set my deviceDeleGate

    strcpy(deviceName,in_name);
    //classType inherit from base
    strcpy(classType, "Analog");
    
    pin = in_pin;
    //not necceary for analog pins
    //pinMode(pin, INPUT);
    strcpy(formula,"");
    strcpy(newFormula,"");
    strcpy(var,"x");
    strcpy(analogValue,"1");
    strcpy(unit,"");
    
}

float Analog::getNewValue() {

    char buffer[10];
    strcpy(analogValue, itoa(analogRead(pin),buffer,10));
    
    if(strcmp(formula,"") != 0) {
        searchAndReplace(var, analogValue);
    }
    
    //Serial.println("new formula ");
    //Serial.println(newFormula);
    float temp = 0.0;
    if(strcmp(newFormula,"") != 0) {
        temp = shuntThis(newFormula);
    }

    return temp;
    
}

const char * Analog::read() {
    
    output[70];
    
    float temp = getNewValue();
    
    dtostrf(temp, 1, 1, output);
    if(strcmp(unit,"") != 0) {
        strcat(output,unit);
    }
    //printf("read output %s\n", output);
    return output;
    
}

void Analog::setFormula(char *in_formula) {
    strcpy(formula, in_formula);
}


char * Analog::getFormula() {
    return formula;
    
}

void Analog::setUnit(char *in_unit) {
    strcpy(unit, in_unit);
}


char * Analog::getUnit() {
    return unit;
    
}


void Analog::loop() {
    
    float temp = getNewValue();
  
    if(strcmp(conditionType, "and") == 0) {
        //prevents rapid switching by offsetting the values
        if(temp < lowerTarget-1) {
            //lower target
            deviceState = false;
            
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


void Analog::searchAndReplace(char *var, char *replace)
{
    //Serial.println("new formula ");
    //Serial.println(newFormula);
    
    strcpy(newFormula, replace_str(formula, var, replace));
    
    //keep searching if more than 1
    while(strstr(newFormula,var)) {
        strcpy(newFormula, replace_str(newFormula, var, replace));
        
    }
}


char * Analog::replace_str(char *str, char *orig, char *rep)
{
    replaceBuffer[70];
    char *p;
    
    if(!(p = strstr(str, orig)))  // Is 'orig' even in 'str'?
        return str;
    
    strncpy(replaceBuffer, str, p-str); // Copy characters from 'str' start to 'orig' st$
    //buffer[p-str] = '\0';
    
    sprintf(replaceBuffer+(p-str), "%s%s", rep, p+strlen(orig));
    
    return replaceBuffer;
}



