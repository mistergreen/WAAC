/*
 AdaFuitAdaFruitPWM4
*/

#ifndef AdaFruitPWM4_h
#define AdaFruitPWM4_h

#include "Arduino.h"
#include "../Device/Device.h" 
#include "../Adafruit_PWMServoDriver/Adafruit_PWMServoDriver.h"

class AdaFruitPWM4 : public Device
{
  public:
    AdaFruitPWM4(char *in_name, int in_dependent_device_id);
    ~AdaFruitPWM4(); // destructor

    void loop(); // required
    void setSuspendTime(boolean in_suspend); //override
    
    void switchOn();
    void switchOff();
    void toggleState();
    
    void setEvent(char *in_string);
    void getEvent(char *string);
    
    int getDependentDevice();
    void setDependentDevice(int id);
    
    int dependentDeviceId;
    Device *dependentDeviceObject;
    
    void setPins(int red, int green, int blue, int white);
    void getPins(int *inArray);
    
    void setPWMs(int in_red, int in_green, int in_blue, int in_white);
    
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
    
    int red[5];
    int green[5];
    int blue[5];
    int white[5];
    
    int initRed;
    int initGreen;
    int initBlue;
    int initWhite;
    
    int currentRed;
    int currentGreen;
    int currentBlue;
    int currentWhite;
    
    int redPin;
    int greenPin;
    int bluePin;
    int whitePin;
    
    long redStartTime;
    long greenStartTime;
    long blueStartTime;
    long whiteStartTime;
    long initMillis;
    char dow[8];
    
    bool oneTime;
    bool redState, greenState, blueState, whiteState;
    
    long fadeSpan;
   
    
};

#endif