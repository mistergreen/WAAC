#ifndef Sensor_h
#define Sensor_h

#include <Arduino.h>

class Sensor
{
  public:
    Sensor();
    
    void setMin(float in_min);
    float getMin();
    
    void setMax(float in_max);
    float getMax();
    
    void setLowerTarget(float in_target);
    float getLowerTarget();
    
    void setHigherTarget(float in_target);
    float getHigherTarget();
    
    void setConditionType(char *string);
    char * getConditionType();

    float lowerTarget;
    float higherTarget;
    float min;
    float max;
    char conditionType[8];

    
  private:
    
    
};

#endif

