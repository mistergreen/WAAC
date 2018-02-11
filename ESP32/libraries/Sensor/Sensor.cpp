//Base Class
//never use by itself. It needs to be included in a Sensor class
// © 2noodles llc
// minh@2noodles.com


#include "Sensor.h"
//#include "shunt.h"
//#include "dtostrf.h"

Sensor::Sensor()
{

    lowerTarget = 40.0;
    higherTarget = 70.0;
    strcpy(conditionType,"and");
    strcpy(formula,"");
    strcpy(newFormula,"");
    strcpy(var,"x");
    strcpy(sensorValue,"0");
    
}


void Sensor::setLowerTarget(float in_target) {
    lowerTarget = in_target;
}

float Sensor::getLowerTarget() {
    return lowerTarget;
}

void Sensor::setHigherTarget(float in_target) {
    higherTarget = in_target;
}

float Sensor::getHigherTarget() {
    return higherTarget;
}

void Sensor::setConditionType(char *string) {
    Serial.print("set: ");
    Serial.println(string);
    strcpy(conditionType, string);
    
}

char * Sensor::getConditionType() {
    Serial.print("get: ");
    Serial.println(conditionType);
    return conditionType;
}


void Sensor::setFormula(char *in_formula) {
    strcpy(formula, in_formula);
}


char * Sensor::getFormula() {
    return formula;
    
}

void Sensor::searchAndReplace(char *var, char *replace)
{
    //Serial.println("new formula ");
    //Serial.println(newFormula);
    
    strcpy(newFormula, replace_str(formula, var, replace));
    
    //keep searching if more than 1
    while(strstr(newFormula,var)) {
        strcpy(newFormula, replace_str(newFormula, var, replace));
        
    }
}


char * Sensor::replace_str(char *str, char *orig, char *rep)
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

float Sensor::getNewValue() {

    float temp = 0.0;

    return temp;
    
}



