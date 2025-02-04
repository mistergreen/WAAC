/*
 Analog
  
*/

#ifndef Analog_h
#define Analog_h


#include <Device.h>
#include <SensorWaac.h>
#include "Storable.h"


class Analog : public Device, public SensorWaac, public Storable
{
  public:
    // Empty constructor for deserialization.
    Analog();

    Analog(char *in_name, uint8_t in_pin);
    ~Analog() {}; // destructor

    void loop(); //required
    const char * read(); // override
    float getNewValue(); // override

    // It serializes the class into a Json document.
    void serialize(
        // Input Json object pointer to be filled with the class information.
        JsonObject& doc);
    
    // It fills the class using the information contained into the document.
    void deserialize(
        // Input Json object pointer containing the class information.
        JsonObject& doc);
    
    unsigned long lastRead = 0;

};//need ; at the end of a class def

#endif