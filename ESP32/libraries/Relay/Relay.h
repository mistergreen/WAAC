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
#include "EventHandler.h"
#include "DeviceDelegate.h"

class Relay : public Device, public Storable, public EventHandler
{
  public:
    Relay();
    Relay(char *in_name, int in_pin, int in_dependent_device_id);
    ~Relay() {}; // destructor

    void loop(); // required
    virtual void switchOn();
    virtual void switchOff();
    virtual void toggleState();
   
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
        
  protected:
    
    // It is the action performed during an event.
    virtual void performActionInEvent();

    // It is the action performed out of an event.
    virtual void performActionOutEvent();

    // It inverts the relay logic.
    bool invert;
    
  private:
    // It stores the relay status.
    bool relayStatus;
    
};//need ; at the end of a class def

#endif