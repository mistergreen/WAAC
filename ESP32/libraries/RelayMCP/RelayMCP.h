/*
 relay
*/

// © 2noodles llc
// minh@2noodles.com

#ifndef RelayMCP_h
#define RelayMCP_h

#include "Arduino.h"
#include "Relay.h" 
#include "Storable.h"
#include "Adafruit_MCP23017.h"

class RelayMCP : public Relay, public Storable
{
  public:
    // Empty constructor.
    RelayMCP();
    RelayMCP(char *in_name, int in_pin, int in_dependent_device_id);
    ~RelayMCP() {}; // destructor

    virtual void switchOn();
    virtual void switchOff();
    void getI2C(int *inArray);
    void setI2C(int insda, int inscl);
    
    // It serializes the class into a Json document.
    void serialize(
    // Input Json object pointer to be filled with the class information.
        JsonObject& doc);
    
    // It fills the class using the information contained into the document.
    void deserialize(
        // Input Json object pointer containing the class information.
        JsonObject& doc);
    
  private:
    boolean isDay;
    boolean onceFlag;
    uint8_t timedIndexCounter;

    int SDA;
    int SCL;
    //Adafruit_MCP23017 mcp; // automatic memory

    
};//need ; at the end of a class def

#endif