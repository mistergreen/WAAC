/*
 AdaFuitAdaFruitPWM8
*/

// © 2noodles llc
// minh@2noodles.com

#ifndef AdaFruitPWM8_h
#define AdaFruitPWM8_h

#define CHANNEL8 8


#include "Arduino.h"
#include <Device.h>
//#include <Adafruit_PWMServoDriver.h>

class AdaFruitPWM8 : public Device
{
  public:
    AdaFruitPWM8(char *in_name, int in_dependent_device_id, int insda, int inscl);
    ~AdaFruitPWM8(); // destructor

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
    
    void setPins(int red, int green, int blue, int white, int aux1, int aux2, int aux3, int aux4);
    void getPins(int *inArray);
    
    void setPWMs(int in_red, int in_green, int in_blue, int in_white, int in_aux1, int in_aux2, int in_aux3, int in_aux4);
    
    void setHasEvent(bool in_hasevent);
   
    void getI2C(int *inArray);
    void setI2C(int insda, int inscl);
    
    
  private:
    
    typedef struct {
        int pwm[4]; // - for each events, value of pwm
        int initColor;
        int currentColor;
        int pin;
        long colorStartTime;
    } colorAux;
    
    colorAux color[CHANNEL8];
    
    //Adafruit_PWMServoDriver *pwmObj; //new() allocated memory
    //Adafruit_PWMServoDriver pwmObj; // automatic memory
    //using PCAhelper.h instead
    
    boolean isDay;
    uint8_t timedIndexCounter;
    
    //have to use int or the parsing bytes wont be correct
    int hour[4];
    int minute[4];
    int second[4];
    int hourDuration[4];
    int minuteDuration[4];
    int secondDuration[4];
    
    int SDA;
    int SCL;

    
    long initMillis;
    
    char dow[8];
    
    bool oneTime;
    bool hasEvent;
    
    long fadeSpan;
    
    //int currentPWM[CHANNEL8];
   
    
};

#endif