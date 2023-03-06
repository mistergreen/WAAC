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

class LightManager : public Device, public Storable
{
  public:
   // The Light Manager custom event manager.
    class LMEventManager : public EventHandler
    {
      public:
        // The class constructor.
        LMEventManager(LightManager* lightManager, int in_dependent_device_id = 0);

        // The class destructor.
        ~LMEventManager();

        // It sets the current color.
        void setCurrentColor(int currentColor);

        // It returns the current color.
        int getCurrentColor();

        // It is the action performed during an event.
        void performActionInEvent();

        // It is the action performed out of an event.
        void performActionOutEvent();

        // It is the action performed not during an event by a dependent device.
        void performActionInEventNoSchedule();

        // It is the action performed out not during an event by a dependent device.
        void performActionOutEventNoSchedule();

        // Deserialize the PWM values from the incoming string.
        void deserializePwms(char *in_string);

        // Serialize the PWM values into the passed string.
        void serializePwms(char *string);

        // The maximum PWM value size.
        static const int sMAX_PWM_VALUE_SIZE = 5;

        // The maximum PWMs buffer size.
        static const int sPWMS_BUFFER_SIZE = sMAX_NUM_EVENTS * sMAX_PWM_VALUE_SIZE;

      private:
        typedef struct {
          int pwm[4]; // - for each events, value of pwm
          int initColor;
          int currentColor;
          long colorStartTime;
        } colorAux;

        colorAux color;

        LightManager* lm;

        uint8_t pwmsIndexCounter;
    };

    // The maximum PWM value size.
    static const int sMAX_PWM_VALUE_SIZE = LMEventManager::sMAX_PWM_VALUE_SIZE;

    // The maximum PWMs buffer size.
    static const int sPWMS_BUFFER_SIZE = LMEventManager::sPWMS_BUFFER_SIZE;

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
    void setEvent(char *in_string);

    // It gets the dark events string.
    virtual void getEvent(char *string);

    // It sets the dark events string.
    void setEventDark(char *in_string);

    // It gets the dark events string.
    virtual void getEventDark(char *string);

    // It switches to dark schedule.
    void switchToDarkMode();

    // It switches to normal schedule.
    void switchToNormalMode();

    // It returns the dark mode value.
    bool getDarkModelMode();

    // It returns the dependendent device ID.
    int getDependentDevice();

    // It sets the dependent device ID.
    void setDependentDevice(int id);

  private:

    // The number of button actions implemented by the device.
    static const int sNUM_BUTTON_ACTIONS = 2;

    // The name of button actions implemented by the device.
    static const char* sNAME_BUTTON_ACTIONS[2];

    static const int sMAX_BIT = pow(2, LEDC_TIMER_BIT) -1;

    long initMillis;

    LMEventManager* currentLM;

    // This class handles the primary schedule for the normal mode.
    LMEventManager* normalEvent;

    // This class handles the secondary schedule for the dark mode.
    LMEventManager* darkEvent;

    // The relay pin.
    int relayPin;

    // The sensor pin.
    int sensorPin;

    bool relayInvert;

    bool relayState;

    // The PWM control pin.
    int pwmPin;

    // The PWM control channel.
    int pwmChannel;

    // The last light sensor value read.
    bool lightSensorValue;

    unsigned long lastDebounceTime;  // the last time the output pin was toggled
    unsigned long debounceDelay; 
    bool lastSensorState;

    // Flag that is true if the dark mode is on.
    bool darkMode;

    // Flag that is true if the manual mode is on.
    bool manualMode;

    bool getNewValue();

    // The realy swithc on command.
    void relaySwitchOn();
  
    // The relay switch off command.
    void relaySwitchOff();
    
    // It sets the pin value.
    virtual void setPin(uint16_t color);
};

#endif