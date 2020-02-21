/*
 Analog
  
*/


#ifndef Analog_h
#define Analog_h


#include <Device.h>
#include <SensorWaac.h>


class Analog : public Device, public SensorWaac
{
  public:
    Analog(char *in_name, uint8_t in_pin);
    ~Analog() {}; // destructor

    void loop(); //required
    const char * read(); // override
    float getNewValue(); // override
};//need ; at the end of a class def

#endif