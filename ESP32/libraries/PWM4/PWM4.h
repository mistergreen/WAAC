/*
 // © 2noodles llc
 // minh@2noodles.com
*/

#ifndef PWM4_h
#define PWM4_h

#define CHANNEL             4
// use 5000 Hz as a LEDC base frequency
#define LEDC_BASE_FREQ      5000
// use bit precission for LEDC timer
#define LEDC_TIMER_BIT      12

#include "Arduino.h"
#include "Device.h" 

class PWM4 : public Device
{
  public:
    PWM4(char *in_name, int in_dependent_device_id);
    ~PWM4(); // destructor

    void loop(); // required
    void setSuspendTime(bool in_suspend); //override
    
    void switchOn();
    void switchOff();
    void toggleState();
    
    void setEvent(char *in_string);
    void getEvent(char *string);
    
    int getDependentDevice();
    void setDependentDevice(int id);
    
    int dependentDeviceId;
    Device *dependentDeviceObject;
    
    void setPins(int red, int green, int blue, int white, int channel0, int channel1, int channel2, int channel3);
    void getPins(int *inPinArray, int *inChannelArray);
    
    void setPWMs(int in_red, int in_green, int in_blue, int in_white);
    
  private:
    
    typedef struct {
        int pwm[4]; // - for each events, value of pwm
        int initColor;
        int currentColor;
        int pin;
        int pwmChannel;
        long colorStartTime;
    } colorAux;
    
    colorAux color[CHANNEL];
    

    bool isDay;
    uint8_t timedIndexCounter;
    int hour[5];
    int minute[5];
    int second[5];
    int hourDuration[5];
    int minuteDuration[5];
    int secondDuration[5];
    
    long initMillis;
    char dow[8];
    bool oneTime;
    
    int maxBit = pow(2, LEDC_TIMER_BIT) -1;
    
};

#endif