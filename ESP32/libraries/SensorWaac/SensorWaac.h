// © 2noodles llc
// minh@2noodles.com

#ifndef SensorWaac_h
#define SensorWaac_h

#include <Arduino.h>

class SensorWaac
{
  public:
    SensorWaac();
    
    void setLowerTarget(float in_target);
    float getLowerTarget();
    
    void setHigherTarget(float in_target);
    float getHigherTarget();
    
    void setConditionType(char *string);
    char * getConditionType();

    float lowerTarget;
    float higherTarget;

    char conditionType[8];

    //
    void setFormula(char *in_formula);
    char * getFormula();
    
    float getNewValue();
    char * replace_str(char *str, char *orig, char *rep);
    void searchAndReplace(char *var, char *replace);
    
    char formula[55];
    char newFormula[100];
    char output[70];
    char replaceBuffer[70];
    char var[2]; // variable to be replace like 'x'
    char sensorValue[10];


    
  private:
    
    
};

#endif

