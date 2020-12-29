/*
  Onewire temp
*/

// © 2noodles llc
// minh@2noodles.com

#ifndef OneWireSensor_h
#define OneWireSensor_h

#include <Arduino.h>
#include <Device.h>
#include <SensorWaac.h>
#include <OneWire.h>

class OneWireSensor : public Device, public SensorWaac
{
  public:
    // The default constructor. 
    // It can be used with the serialize method to pass the parameters and instantiate OneWire class.
    OneWireSensor();
    
    OneWireSensor(char *in_name, uint8_t in_pin, float in_min, float in_max);
    ~OneWireSensor(); // destructor
    
    //****** required **********
    void loop();
    const char * read(); // override
    float getNewValue(); // override
    //******************

    void setF(bool in_state);
    bool getF();
   
    // It serializes the class into a Json document.
    void serialize(
    // Input Json object pointer to be filled with the class information.
        JsonObject& doc);
    
    // It fills the class using the information contained into the document.
    void deserialize(
        // Input Json object pointer containing the class information.
        JsonObject& doc);
    
  private:

    bool fahrenheit;
    char temperature[17];
    OneWire *ds;

};

#endif