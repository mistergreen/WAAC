/*
 InputMCP
// © 2noodles llc
// minh@2noodles.com
*/


#ifndef InputMCP_h
#define InputMCP_h


#include <Device.h>
#include <SensorWaac.h>
#include "Adafruit_MCP23017.h"


class InputMCP : public Device, public SensorWaac
{
  public:
    InputMCP(char *in_name, uint8_t in_pin);
    ~InputMCP() {}; // destructor

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