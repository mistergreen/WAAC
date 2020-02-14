/*
 relay
*/

// © 2noodles llc
// minh@2noodles.com

#ifndef Relay_h
#define Relay_h

#include "Arduino.h"
#include "Device.h"
#include "Storable.h"

class Relay : public Device, public Storable
{
  public:
    Relay();
    Relay(char *in_name, int in_pin, int in_dependent_device_id);
    ~Relay() {}; // destructor

    void loop(); // required
    virtual void switchOn();
    virtual void switchOff();
    virtual void toggleState();
    
    void setEvent(char *in_string); // override
    void getEvent(char *string); // override
    
    int getDependentDevice(); // override
    void setDependentDevice(int id); // override
    
    int dependentDeviceId;
    Device *dependentDeviceObject;
    
    boolean invert;
    boolean getInvert();
    void setInvert(boolean state);
    
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
    int hour[5];
    int minute[5];
    int second[5];
    int hourDuration[5];
    int minuteDuration[5];
    int secondDuration[5];
    char dow[5][8];
    
};//need ; at the end of a class def

#endif