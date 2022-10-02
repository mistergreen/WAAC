// © 2noodles llc
// minh@2noodles.com


#ifndef Device_h
#define Device_h

#include "Storable.h"
#include "Arduino.h"

class Device : public Storable
{
  public:
    Device();
    virtual ~Device() {}; //destructor
    virtual void loop() = 0; //child must define - will be called from delegate loop()
    
    //can override
    virtual void setSuspendTime(bool in_suspend) {};
    virtual const char * read(){ return "0.0"; }; 
    virtual void toggleState(){};

    //
    virtual bool isI2C() { return false; };
    virtual bool isVideo() { return false; };
    virtual bool isAlert() { return false; };
    virtual bool isSensor() { return false; };

    //child will inherit
    //get from Sensor to expose to Device
    void setMin(float in_min);
    float getMin();
    
    void setMax(float in_max);
    float getMax();

    void setUnit(char *in_unit);
    char * getUnit();

    //

    void stripTime(char *inString, int *inArray);
    
    char * getClassName();
    
    char * getDeviceName();
    void setDeviceName(char *name);
    
    int getPin();
    void setPin(int in_pin);
    
    int getDeviceId();
    void setDeviceId(int id);

    void setStyle(char *image, char *color);
    void setImageName(char *image);
    void setDeviceColor(char *color);
    char * getImageName();
    char * getDeviceColor();
    
    bool getDeviceState();
    void setDeviceState(bool state);
    
    long convertToSeconds(int in_hour, int in_minute, int in_second);

    // It returns how many callable actions by a button are implemented by the device.
    virtual int getNumButtonActions();

    // It returns the name of an action specified by the passed actionId
    virtual const char* getButtonActionName(
      // The action to call
      int actionId
    );

    // It calls an action specified by the passed actionId
    virtual void callButtonAction(
      // The action to call
      int actionId
    );

    // It serializes the class into a Json document.
    virtual void serialize(
    // Input Json object pointer to be filled with the class information.
        JsonObject& doc);
    
    // It fills the class using the information contained into the document.
    virtual void deserialize(
        // Input Json object pointer containing the class information.
        JsonObject& doc);

    // The number of button actions implemented by the device.
    static const int NUM_BUTTON_ACTIONS;

    // The name of button actions implemented by the device.
    static const char* NAME_BUTTON_ACTIONS;

    int deviceId;
    float min;
    float max;
    char unit[35];
    
  protected:
    int pin;
    char classType[20];
    char deviceName[35];
    char imageName[20];
    char deviceColor[7];
    bool deviceState;
    
  private:

};

#endif

