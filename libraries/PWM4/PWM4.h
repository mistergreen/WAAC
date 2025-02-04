/*
 // © 2noodles llc
 // minh@2noodles.com
*/

#ifndef PWM4_h
#define PWM4_h

// use 5000 Hz as a LEDC base frequency
#define LEDC_BASE_FREQ      5000
// use bit precission for LEDC timer
#define LEDC_TIMER_BIT      12

#include "Arduino.h"
#include "Device.h"
#include "EventHandler.h"
#include "DeviceDelegate.h"

class PWM4 : public Device, public EventHandler, public Storable
{
  public:

    // Class constructor, it sets the default values.
    PWM4(char *in_name = "", int in_dependent_device_id = 0, uint8_t channels = 4);
    //PWM4(); // don't need to define? I'm getting ambiguous call if defined
    ~PWM4(); // destructor
    
    void switchOn();
    void switchOff();
    void toggleState();

    void loop(); // required

    void setPins(int red, int green, int blue, int white, int channel0, int channel1, int channel2, int channel3);
    void getPins(int *inPinArray, int *inChannelArray);
    
    void setPWMs(int in_red, int in_green, int in_blue, int in_white);

    // Set the colors for each event.
    void setEventColors(char *in_string);

    // Get the colors of the events.
    void getEventColors(char *string);

    // It serializes the class into a Json document.
    void serialize(
        // Input Json object pointer to be filled with the class information.
        JsonObject& doc);
    
    // It fills the class using the information contained into the document.
    void deserialize(
        // Input Json object pointer containing the class information.
        JsonObject& doc);
    
  protected:
    // The number of channels
    // IS it defined somewhere? // defined in constructor, channels
    const uint8_t NUM_CHANNELS; 

    typedef struct {
        int pwm[4]; // - for each events, value of pwm
        int initColor;
        int currentColor;
        int pin;
        int pwmChannel;
        long colorStartTime;
    } colorAux;
    
    colorAux* color;

    // It is the action performed during an event.
    virtual void performActionInEvent();

    // It is the action performed out of an event.
    virtual void performActionOutEvent();

    // It sets the pin value.
    virtual void setPin(uint8_t channel, uint16_t color);

  private:
    
    uint8_t colorsIndexCounter;

    long initMillis;
    
    int maxBit = pow(2, LEDC_TIMER_BIT) -1;
    
    // It is the action performed not during an event by a dependent device.
    virtual void performActionInEventNoSchedule();

    // It is the action performed out not during an event by a dependent device.
    virtual void performActionOutEventNoSchedule();
};

#endif