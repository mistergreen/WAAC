/*
 relay
*/

// © 2noodles llc
// minh@2noodles.com

#ifndef RelayMCP_h
#define RelayMCP_h

#include "Arduino.h"
#include "Relay.h" 
#include "Adafruit_MCP23017.h"

class RelayMCP : public Relay
{
  public:
    RelayMCP(char *in_name, int in_pin, int in_dependent_device_id);
    ~RelayMCP() {}; // destructor

    virtual void switchOn();
    virtual void switchOff();
    void getI2C(int *inArray);
    void setI2C(int insda, int inscl);
    
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

    int SDA;
    int SCL;
    //Adafruit_MCP23017 mcp; // automatic memory

    
};//need ; at the end of a class def

#endif