// © 2noodles llc
// minh@2noodles.com

#ifndef SensorWaac_h
#define SensorWaac_h

#include "Storable.h"
#include <Arduino.h>

class SensorWaac : public Storable
{
  public:
    SensorWaac();
    
    // It serializes the class into a Json document.
    void serialize(
    // Input Json object pointer to be filled with the class information.
        JsonObject& doc);
    
    // It fills the class using the information contained into the document.
    void deserialize(
        // Input Json object pointer containing the class information.
        JsonObject& doc);
    
    void setLowerTarget(float in_target);
    float getLowerTarget();
    
    void setHigherTarget(float in_target);
    float getHigherTarget();
    
    void setConditionType(char *string);
    char * getConditionType();

    void setFormula(char *in_formula);
    char * getFormula();
    
    float getNewValue();
    char * replace_str(char *str, char *orig, char *rep);
    void searchAndReplace(char *var, char *replace);
    
  protected:
  
    float lowerTarget;
    float higherTarget;

    char conditionType[8];
    
    char formula[55];
    char newFormula[100];
    char output[70];
    char var[2]; // variable to be replace like 'x'
    char sensorValue[10];
    
  private:
  
    char replaceBuffer[70];
};

#endif

