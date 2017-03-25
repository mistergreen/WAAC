/*
 Analog
  
*/
// © 2noodles llc
// minh@2noodles.com

#ifndef Analog_h
#define Analog_h


#include <Device.h>
#include <Sensor.h>
#include <Arduino.h>
#include <Shunt.h>


class Analog : public Device, public Sensor
{
  public:
    Analog(char *in_name, uint8_t in_pin);
    ~Analog() {}; // destructor
    

    void loop(); // required
    const char * read(); // override
    
    void setFormula(char *in_formula);
    char * getFormula();
    
    void setUnit(char *in_unit);
    char * getUnit();
    
    float getNewValue();
    
    //void searchReplace(const char *str, const char *old, const char *replace, char *newStr);
    char * replace_str(char *str, char *orig, char *rep);
    void searchAndReplace(char *var, char *replace);
    
    char formula[46];
    char newFormula[100];
    char output[70];
    char replaceBuffer[70];
    char unit[35];
    char var[2];
    char analogValue[10];

    
};//need ; at the end of a class def

#endif