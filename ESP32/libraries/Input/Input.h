/*
 Input
// © 2noodles llc
// minh@2noodles.com
*/


#ifndef Input_h
#define Input_h


#include <Device.h>
#include <SensorWaac.h>
#include <Storable.h>


class Input : public Device, public SensorWaac, public Storable
{
  public:
    Input(char *in_name, uint8_t in_pin);
    ~Input() {}; // destructor
    
    // It serializes the class into a Json document.
    void serialize(
    // Input Json object pointer to be filled with the class information.
        JsonObject& doc);
    
    // It fills the class using the information contained into the document.
    void deserialize(
        // Input Json object pointer containing the class information.
        JsonObject& doc);

    void loop(); //required
    const char * read(); // override
    bool getNewValue(); // override

  private:
    unsigned long lastDebounceTime;  // the last time the output pin was toggled
    unsigned long debounceDelay;  
    bool lastState;
    
};//need ; at the end of a class def

#endif