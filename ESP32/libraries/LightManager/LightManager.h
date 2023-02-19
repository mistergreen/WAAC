/*
 // © Andrea Villa
// villa.andrea@gmail.com
*/

#ifndef LIGHTMANAGER_h
#define LIGHTMANAGER_h

// use 5000 Hz as a LEDC base frequency
#define LEDC_BASE_FREQ      5000
// use bit precission for LEDC timer
#define LEDC_TIMER_BIT      12

#include "Arduino.h"
#include "Device.h"
#include "EventHandler.h"
#include "DeviceDelegate.h"

class LightManager : public Device, public EventHandler, public Storable
{
  public:
    // The maximum PWM value size.
    static const int sMAX_PWM_VALUE_SIZE = 5;

    // The maximum PWMs buffer size.
    static const int sPWMS_BUFFER_SIZE = sMAX_NUM_EVENTS * sMAX_PWM_VALUE_SIZE;

    // Class constructor, it sets the default values.
    LightManager(char *in_name = "", int in_dependent_device_id = 0);

    ~LightManager(); // destructor
    
    void switchOn();
    void switchOff();
    void toggleState();

    boolean getInvert();
    void setInvert(boolean state);

    // Set if events can be suspended.
    virtual void setSuspendTime(bool in_suspend);
    
    // It reurns if the timer is suspended.
    virtual bool getSuspendTime();

    void loop(); // required

    // It sets the voltage control pin,
    void setLightControlPin(int pin, int channel);
    
    // It gets the voltage control pin.
    void getLightControlPin(int *pin, int *channel);

    // It sets the relay control pin,
    void setRelayPin(int pin);
    
    // It gets the relay control pin.
    int getRelayPin();

    // It sets the light sensor pin,
    void setSensorPin(int pin);
    
    // It gets the light sensor pin.
    int getSensorPin();

    // It sets direcctly the PWM value.
    void setPWM(int pwm);

    // Deserialize the PWM values from the incoming string.
    void deserializePwms(char *in_string);

    // Serialize the PWM values into the passed string.
    void serializePwms(char *string);

    // Deserialize the PWM dark values from the incoming string.
    void deserializeDarkPwms(char *in_string);

    // Serialize the PWM dark values into the passed string.
    void serializeDarkPwms(char *string);

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

    // It sets the dark events string.
    void setEventDark(char *in_string);

    // It gets the dark events string.
    virtual void getEventDark(char *string);
    
  protected:
  
    typedef struct {
        int pwm[4]; // - for each events, value of pwm
        int initColor;
        int currentColor;
        int pin;
        int pwmChannel;
        long colorStartTime;
    } colorAux;
    
    colorAux color;

    colorAux colorDark;

    // It is the action performed during an event.
    virtual void performActionInEvent();

    // It is the action performed out of an event.
    virtual void performActionOutEvent();

  private:
    // The number of button actions implemented by the device.
    static const int sNUM_BUTTON_ACTIONS = 2;

    // The name of button actions implemented by the device.
    static const char* sNAME_BUTTON_ACTIONS[2];

    static const int sMAX_BIT = pow(2, LEDC_TIMER_BIT) -1;

    uint8_t pwmsIndexCounter;

    uint8_t pwmsDarkIndexCounter;

    long initMillis;

    // This class handles the secondary schedule for the away mode.
    EventHandler darkEvent;

    // The relay pin.
    int relayPin;

    // The sensor pin.
    int sensorPin;

    bool relayInvert;

    bool relayState;

    unsigned long lastDebounceTime;  // the last time the output pin was toggled
    unsigned long debounceDelay; 
    bool lastSensorState;

    
    bool getNewValue();

    // The realy swithc on command.
    void relaySwitchOn();
  
    // The relay switch off command.
    void relaySwitchOff();
    
    // It is the action performed not during an event by a dependent device.
    virtual void performActionInEventNoSchedule();

    // It is the action performed out not during an event by a dependent device.
    virtual void performActionOutEventNoSchedule();

    // It sets the pin value.
    virtual void setPin(uint8_t channel, uint16_t color);
};

#endif