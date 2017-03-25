//Base Class
//never use by itself. It needs to be included in a Sensor class
// © 2noodles llc
// minh@2noodles.com

#include "Sensor.h"


Sensor::Sensor()
{
    min = 0.0;
    max = 100.0;
    lowerTarget = 40.0;
    higherTarget = 70.0;
    strcpy(conditionType,"and");
}

void Sensor::setMin(float in_min) {
    min = in_min;
}

float Sensor::getMin() {
    return min;
}

void Sensor::setMax(float in_max) {
    max = in_max;
}

float Sensor::getMax() {
    return max;
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



