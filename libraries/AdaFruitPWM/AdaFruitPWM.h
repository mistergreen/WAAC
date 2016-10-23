/*
 PWM
*/
#ifndef AdaFruitPWM_h
#define AdaFruitPWM_h

#include "Arduino.h"
#include "../Device/Device.h" 
#include "../Adafruit_PWMServoDriver/Adafruit_PWMServoDriver.h"

class AdaFruitPWM : public Device
{
  public:
    AdaFruitPWM(char *in_name, int in_pin, int in_dependent_device_id);
    ~AdaFruitPWM(); // destructor

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
    void setPWM(int in_pwm);
    
  private:
    Adafruit_PWMServoDriver *pwmObj;
    boolean isDay;
    uint8_t timedIndexCounter;
    uint8_t hour[5];
    uint8_t minute[5];
    uint8_t second[5];
    uint8_t hourDuration[5];
    uint8_t minuteDuration[5];
    uint8_t secondDuration[5];
    
    int pwmValue[5];
    int initPWM;
    
    int currentPWM;
    char readOut[10];
    
    long startTime;
    long initMillis;
    char dow[8];
    
    long fadeSpan;
    int pwmDif; // can be + -
    long interval; // in ms
    bool oneTime;

    
};

#endif