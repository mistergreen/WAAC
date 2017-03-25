/*
 relay
*/

// © 2noodles llc
// minh@2noodles.com

#ifndef ArduinoRelay_h
#define ArduinoRelay_h

#include "Arduino.h"
#include "Relay.h"

class ArduinoRelay : public Relay
{
  public:
    ArduinoRelay(char *in_name, int in_pin, int in_dependent_device_id);
    ~ArduinoRelay() {}; // destructor
    void switchOn();
    void switchOff();
    void toggleState();
    
  private:
    boolean isDay;
    boolean onceFlag;
    uint8_t timedIndexCounter;
    int hour[5];
    int minute[5];
    int second[5];
    int hourDuration[5];
    int minuteDuration[5];
    int secondDuration[5];
    char dow[5][8];

    
};//need ; at the end of a class def

#endif