// © 2noodles llc
// minh@2noodles.com

/*

 HallSensor
  
*/


#ifndef HallSensor_h
#define HallSensor_h


#include <Device.h>
#include <SensorWaac.h>
#include <Storable.h>


class HallSensor : public Device, public SensorWaac
{
  public:
    HallSensor(char *in_name, uint8_t in_pin);
    ~HallSensor(); // destructor

    // It serializes the class into a Json document.
    void serialize(
    // Input Json object pointer to be filled with the class information.
        JsonObject& doc);
    
    // It fills the class using the information contained into the document.
    void deserialize(
        // Input Json object pointer containing the class information.
        JsonObject& doc);
        
    void loop(); // required
    void setup(void (*ISR_callback)(void), int value);
    const char * read(); // override
    float getNewValue(); // override

    bool isSensor() {return true;};

    void rpm ();

    volatile int count; 
    unsigned long lastRead; 
    float sensorVal;

    
};//need ; at the end of a class def

#endif