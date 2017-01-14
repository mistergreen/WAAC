/*
  Onewire temp
*/
#ifndef OneWireSensor_h
#define OneWireSensor_h

#include <Arduino.h>
#include <Device.h>
#include <Sensor.h>
#include <OneWire.h>

class OneWireSensor : public Device, public Sensor
{
  public:
    OneWireSensor(char *in_name, uint8_t in_pin, float in_min, float in_max);
    ~OneWireSensor(); // destructor
    
    //****** required **********
    void loop();
    const char * read(); // override
    
    //******************
    
    float getTemp();
    
    void setF(bool in_state);
    bool getF();
    
  private:
    //uint8_t pin;
    bool fahrenheit;
    char temperature[17];
    OneWire *ds;

};

#endif