/*
 ServoMotor
*/

// © 2noodles llc
// minh@2noodles.com
// Andrea Villa
// villa.andrea@gmail.com

#ifndef ServoMotor_h
#define ServoMotor_h

#include "Arduino.h"
#include "Device.h" 
#include "Storable.h"
#include "EventHandler.h"

#include "Servo.h"
#include "ArduinoJson.h"

class ServoMotor : public Device, public Storable, public EventHandler
{
  public:
    ServoMotor();
    ServoMotor(char *in_name, int in_pin, int in_dependent_device_id, int stop_angle, int move_angle, int second_duration);
    ~ServoMotor() {}; // destructor

    void loop(); // required

    virtual void toggleState();

    int getDuration();
    void setDuration(int duration);
    
    uint8_t getStopAngle();
    void setStopAngle(uint8_t angle);

    uint8_t getMoveAngle();
    void setMoveAngle(uint8_t angle);
    
    void setPin(int in_pin);
    
    // It serializes the class into a Json document.
    void serialize(
    // Input Json object pointer to be filled with the class information.
        JsonObject& doc);
    
    // It fills the class using the information contained into the document.
    void deserialize(
        // Input Json object pointer containing the class information.
        JsonObject& doc);
    
  private:
    // The servo motor controller.
    Servo servo;
    uint8_t stopAngle;
    uint8_t moveAngle;
    int secondDuration;

    uint8_t timedIndexCounter;
    // The current triggered serving time. It is needed to trigger the servo motor only once.
    long servingTime;

    virtual void switchOn();

    // It is the action performed during an event.
    virtual void performActionInEvent();

    // It is the action performed out of an event.
    virtual void performActionOutEvent();
};//need ; at the end of a class def

#endif