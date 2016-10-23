/*
 relay
*/
#ifndef Relay_h
#define Relay_h

#include "Arduino.h"
#include "../Device/Device.h" 

class Relay : public Device
{
  public:
    Relay(char *in_name, int in_pin, int in_dependent_device_id);
    ~Relay() {}; // destructor

    void loop(); // required
    void switchOn();
    void switchOff();
    void toggleState();
    
    void setEvent(char *in_string); // override
    void getEvent(char *string); // override
    
    int getDependentDevice(); // override
    void setDependentDevice(int id); // override
    
    int dependentDeviceId;
    Device *dependentDeviceObject;
    
    
  private:
    boolean isDay;
    uint8_t timedIndexCounter;
    uint8_t hour[5];
    uint8_t minute[5];
    uint8_t second[5];
    uint8_t hourDuration[5];
    uint8_t minuteDuration[5];
    uint8_t secondDuration[5];
    char dow[5][8];
    
};//need ; at the end of a class def

#endif