/*
 InputMCP
// © 2noodles llc
// minh@2noodles.com
*/


#ifndef InputMCP_h
#define InputMCP_h


#include <Device.h>
#include <SensorWaac.h>
#include <Storable.h>
#include "Adafruit_MCP23017.h"


class InputMCP : public Device, public SensorWaac, public Storable
{
  public:
    InputMCP(char *in_name, uint8_t in_pin);
    ~InputMCP() {}; // destructor
    
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
    void setI2C(int insda, int inscl); 
    void getI2C(int *inArray);

  private:
    
    unsigned long lastDebounceTime;  // the last time the output pin was toggled
    unsigned long debounceDelay;  
    bool lastState;
    int SDA;
    int SCL;
    //Adafruit_MCP23017 mcp; // automatic memory
    
};//need ; at the end of a class def

#endif