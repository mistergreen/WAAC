/*
 Input
// © 2noodles llc
// minh@2noodles.com
*/


#ifndef Input_h
#define Input_h


#include <Device.h>
#include <SensorWaac.h>


class Input : public Device, public SensorWaac
{
  public:
    Input(char *in_name, uint8_t in_pin);
    ~Input() {}; // destructor

    void loop(); //required
    const char * read(); // override
    bool getNewValue(); // override

  private:
    unsigned long lastDebounceTime;  // the last time the output pin was toggled
    unsigned long debounceDelay;  
    bool lastState;
    
};//need ; at the end of a class def

#endif