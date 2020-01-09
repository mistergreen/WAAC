//Base Class
//never use by itself. It needs to be included in a SensorWaac class
// © 2noodles llc
// minh@2noodles.com


#include "SensorWaac.h"
//#include "shunt.h"
//#include "dtostrf.h"

SensorWaac::SensorWaac()
{

    lowerTarget = 40.0;
    higherTarget = 70.0;
    strcpy(conditionType,"and");
    strcpy(formula,"");
    strcpy(newFormula,"");
    strcpy(var,"x");
    strcpy(sensorValue,"0");
    
}


void SensorWaac::setLowerTarget(float in_target) {
    lowerTarget = in_target;
}

float SensorWaac::getLowerTarget() {
    return lowerTarget;
}

void SensorWaac::setHigherTarget(float in_target) {
    higherTarget = in_target;
}

float SensorWaac::getHigherTarget() {
    return higherTarget;
}

void SensorWaac::setConditionType(char *string) {
    Serial.print("set: ");
    Serial.println(string);
    strcpy(conditionType, string);
    
}

char * SensorWaac::getConditionType() {
    Serial.print("get: ");
    Serial.println(conditionType);
    return conditionType;
}


void SensorWaac::setFormula(char *in_formula) {
    strcpy(formula, in_formula);
}


char * SensorWaac::getFormula() {
    return formula;
    
}

void SensorWaac::searchAndReplace(char *var, char *replace)
{
    //Serial.println("new formula ");
    //Serial.println(newFormula);
    
    strcpy(newFormula, replace_str(formula, var, replace));
    
    //keep searching if more than 1
    while(strstr(newFormula,var)) {
        strcpy(newFormula, replace_str(newFormula, var, replace));
        
    }
}


char * SensorWaac::replace_str(char *str, char *orig, char *rep)
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

float SensorWaac::getNewValue() {

    float temp = 0.0;

    return temp;
    
}



