// Andrea Villa
// villa.andrea@gmail.com

#ifndef InputButton_h
#define InputButton_h


#include <Device.h>
#include <Storable.h>
#include "Button2.h"


class InputButton : public Device, public Storable
{
  public:
    // Empty constructor for deserialization.
    InputButton();

    // Constructor
    InputButton(char *in_name, uint8_t in_pin);
    
    ~InputButton() {}; // destructor
    
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

    typedef void (*CallbackFunction)();

    void setClickCallback(CallbackFunction f);

    // It sets the click device and action.
    void setClickAction(int device, int action);
    
    // It sets the double click device and action.
    void setDoubleClickAction(int device, int action);
    
    // It sets the triple click device and action.
    void setTripleClickAction(int device, int action);
    
    // It sets the long click device and action.
    void setLongClickAction(int device, int action);

    // It gets the click device.
    int getClickDevice();
    
    // It gets the double click device.
    int getDoubleClickDevice();
    
    // It gets the triple click device.
    int getTripleClickDevice();
    
    // It gets the long click device.
    int getLongClickDevice();

    // It gets the click action.
    int getClickAction();
    
    // It gets the double click action.
    int getDoubleClickAction();
    
    // It gets the triple click action.
    int getTripleClickAction();
    
    // It gets the long click action.
    int getLongClickAction();
  private:

    // It performs actions on the sytem by simulating a device.
    class SystemDevice : public Device
    {
      // It calls an action specified by the passed actionId
      void callButtonAction(
        // The action to call
        int actionId);

      void loop() {};
    };

    // The button handler class.
    Button2 button;

    // The system device.
    SystemDevice systemDevice;

    char output[5];

    // It contains the passed actions.
    int actions[4];

    // It contains the passed devices
    Device* devices[4];

    // It contains the passed devices ids
    int devicesId[4];
};

#endif