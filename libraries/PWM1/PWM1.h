/*
 PWM
*/
#ifndef PWM1_h
#define PWM1_h

#include "Arduino.h"
#include "../Device/Device.h" 

class PWM1 : public Device
{
  public:
    PWM1(char *in_name, int in_pin, int in_dependent_device_id);
    ~PWM1() {}; // destructor

    void loop();
    void switchOn();
    void switchOff();
    void toggleState();
    
    void setEvent(char *in_string);
    void getEvent(char *string);
    
    int getDependentDevice();
    void setDependentDevice(int id);
    
    int dependentDeviceId;
    Device *dependentDeviceObject;
    
    void setSuspendTime(boolean in_suspend);
    void setPWM(uint8_t in_pwm);
    
  private:
    
    boolean isDay;
    uint8_t timedIndexCounter;
    uint8_t hour[5];
    uint8_t minute[5];
    uint8_t second[5];
    uint8_t hourDuration[5];
    uint8_t minuteDuration[5];
    uint8_t secondDuration[5];
    uint8_t pwmValue[5];
    uint8_t initPWM;
    
    int currentPWM;
    char readOut[10];
    
    long startTime;
    long initMillis;
    char dow[8];
    bool oneTime;
    
};

#endif