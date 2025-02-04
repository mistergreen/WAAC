/*
 StepperMotor
*/

// Andrea Villa
// villa.andrea@gmail.com

#ifndef StepperMotor_h
#define StepperMotor_h

#include "Arduino.h"
#include "Device.h" 
#include "Storable.h"
#include "EventHandler.h"

#include "Stepper.h"
#include "ArduinoJson.h"

class StepperMotor : public Device, public Storable, public EventHandler
{
  public:
    StepperMotor();
    StepperMotor(char *in_name, 
                 int in_pin0, 
                 int in_pin1, 
                 int in_pin2, 
                 int in_pin3, 
                 int in_dependent_device_id, 
                 int total_steps, 
                 int steps_per_action, 
                 int speed_rpm);
                 
    ~StepperMotor() {}; // destructor

    void loop(); // required

    virtual void toggleState();

    int getSteps();
    void setSteps(int steps_per_action);

    int getTotalSteps();
    void setTotalSteps(int total_steps);

    int getSpeed();
    void setSpeed(int speed_rpm);

    void setPins(int in_pin0, int in_pin1, int in_pin2, int in_pin3);

    int getPin(int pinNum);
    
    // It resets the class counters.
    void reset();

    // It send one motor loop.
    void oneLoop();

    // It serializes the class into a Json document.
    void serialize(
    // Input Json object pointer to be filled with the class information.
        JsonObject& doc);
    
    // It fills the class using the information contained into the document.
    void deserialize(
        // Input Json object pointer containing the class information.
        JsonObject& doc);
    
  private:
    // The maximum pin number.
    static const int sMAX_PIN = 3;

    // The stepper motor controller.
    Stepper stepper;

    // The pins associated with the controller.
    int pins[4];

    // The toatl number of steps in 360 degrees.
    int totalSteps;
    
    // The number of steps to divide the 360 degrees.
    int stepsDivision;

    // The number of triggers performed.
    int performedTriggers;

    // The number of steps performed.
    int performedSteps;

    // The speed of the motor in rpm.
    int speedRpm;

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