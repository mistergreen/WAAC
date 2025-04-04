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
    // Set if events can be suspended.
    virtual void setSuspendTime(bool in_suspend);
    // It reurns if the timer is suspended.
    virtual bool getSuspendTime();
   
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

        // It returns how many callable actions by a button are implemented by the device.
    int getNumButtonActions();

    // It returns the name of an action specified by the passed actionId
    const char* getButtonActionName(
      // The action to call
      int actionId
    );

    // It calls an action specified by the passed actionId
    void callButtonAction(
      // The action to call
      int actionId
    );
        
  protected:
    
    // It is the action performed during an event.
    virtual void performActionInEvent();

    // It is the action performed out of an event.
    virtual void performActionOutEvent();

    // It inverts the relay logic.
    bool invert;
    
  private:
      // The number of button actions implemented by the device.
    static const int sNUM_BUTTON_ACTIONS = 2;

    // The name of button actions implemented by the device.
    static const char* sNAME_BUTTON_ACTIONS[2];

    // It stores the relay status.
    bool relayStatus;
    
};//need ; at the end of a class def

#endif