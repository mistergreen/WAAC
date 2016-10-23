/*
 PWM4
*/

#ifndef PWM4_h
#define PWM4_h

#include "Arduino.h"
#include "../Device/Device.h" 

class PWM4 : public Device
{
  public:
    PWM4(char *in_name, int in_dependent_device_id);
    ~PWM4() {}; // destructor

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
    
    void setPins(uint8_t red, uint8_t green, uint8_t blue, uint8_t white);
    void getPins(uint8_t *inArray);
    
    void setPWMs(uint8_t in_red, uint8_t in_green, uint8_t in_blue, uint8_t in_white);
    
  private:

    boolean isDay;
    uint8_t timedIndexCounter;
    uint8_t hour[5];
    uint8_t minute[5];
    uint8_t second[5];
    uint8_t hourDuration[5];
    uint8_t minuteDuration[5];
    uint8_t secondDuration[5];
    
    uint8_t red[5];
    uint8_t green[5];
    uint8_t blue[5];
    uint8_t white[5];
    
    uint8_t initRed;
    uint8_t initGreen;
    uint8_t initBlue;
    uint8_t initWhite;
    
    int currentRed;
    int currentGreen;
    int currentBlue;
    int currentWhite;
    
    uint8_t redPin;
    uint8_t greenPin;
    uint8_t bluePin;
    uint8_t whitePin;
    
    long redStartTime;
    long greenStartTime;
    long blueStartTime;
    long whiteStartTime;
    long initMillis;
    char dow[8];
    bool oneTime;
    
};

#endif