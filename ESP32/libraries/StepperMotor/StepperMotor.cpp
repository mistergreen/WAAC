/*
  StepperMotor
*/

// Andrea Villa
// villa.andrea@gmail.com

#include "StepperMotor.h"
#include "DeviceDelegate.h"
#include "Stepper.h"

StepperMotor::StepperMotor() : Device(), EventHandler()
{
    totalSteps = 0;
    stepsPerAction = 0;
    speedRpm = 0;
    
    stepper = Stepper();
}

StepperMotor::StepperMotor(char *in_name, 
                           int in_pin0, 
                           int in_pin1, 
                           int in_pin2, 
                           int in_pin3, 
                           int in_dependent_device_id, 
                           int total_steps, 
                           int steps_per_action, 
                           int speed_rpm) : Device(), EventHandler(in_dependent_device_id)
{
    strcpy(deviceName, in_name);
    //classType inherit from base
    strcpy(classType, "StepperMotor");

    pins[0] = in_pin0;
    pins[1] = in_pin1;
    pins[2] = in_pin2;
    pins[3] = in_pin3;
    
    totalSteps = total_steps;
    stepsPerAction = steps_per_action;
    speedRpm = speed_rpm;
    
    // Connect the stepper library to the selected pins.
    Serial.print("Setting stepper: ");
    Serial.print(totalSteps);
    Serial.print(" pins: ");
    Serial.print(pins[0]);
    Serial.print(", ");
    Serial.print(pins[1]);
    Serial.print(", ");
    Serial.print(pins[2]);
    Serial.print(", ");
    Serial.print(pins[3]);
    Serial.println();

    stepper = Stepper(totalSteps, pins[0], pins[1], pins[2], pins[3]);
    stepper.setSpeed(speedRpm);
}


void StepperMotor::loop()
{
    EventHandler::loop();
}

void StepperMotor::performActionInEvent()
{
    // Change state only if needed, so only once.
    if (false == deviceState)
    {
        switchOn();

        deviceState = true;
    }
}


void StepperMotor::performActionOutEvent()
{
    deviceState = false;
}


int StepperMotor::getSteps() {
    return stepsPerAction;
    
}

void StepperMotor::setSteps(int steps_per_action) {
    stepsPerAction = steps_per_action;
}

int StepperMotor::getTotalSteps() {
    return totalSteps;
    
}

void StepperMotor::setTotalSteps(int total_steps) {
    totalSteps = total_steps;
    stepper.setNumberOfSteps(totalSteps);
}


int StepperMotor::getSpeed() {
    return speedRpm;
    
}

void StepperMotor::setSpeed(int speed_rpm) {
    speedRpm = speed_rpm;
    stepper.setSpeed(speedRpm);
}

void StepperMotor::setPins(int in_pin0, int in_pin1, int in_pin2, int in_pin3) {

    pins[0] = in_pin0;
    pins[1] = in_pin1;
    pins[2] = in_pin2;
    pins[3] = in_pin3;
    
    // Connect the stepper library to the selected pin.
    stepper.setPins(pins[0], pins[1], pins[2], pins[3]);

    // Connect the stepper library to the selected pins.
    Serial.print("Setting stepper: ");
    Serial.print(totalSteps);
    Serial.print(" pins: ");
    Serial.print(pins[0]);
    Serial.print(", ");
    Serial.print(pins[1]);
    Serial.print(", ");
    Serial.print(pins[2]);
    Serial.print(", ");
    Serial.print(pins[3]);
    Serial.println();
}

int StepperMotor::getPin(int pin_num)
{
    // Climp the pin number.
    if (pin_num > sMAX_PIN)
    {
        pin_num = sMAX_PIN;
    }

    return pins[pin_num];
}
	
void StepperMotor::switchOn()
{
    Serial.println("StepperMotor::switchOn switching on");

    // Trigger the servo motor.
    Serial.println("switching on");
    Serial.print("Setting steps: ");
    Serial.println(stepsPerAction);
    stepper.step(stepsPerAction);
    delay(500);
    Serial.println("Steps completed");
}


void StepperMotor::toggleState()
{
    // Send one event only if there is no event in progress
    if (false == deviceState) {
        switchOn();
    }
}


void StepperMotor::serialize(JsonObject& doc)
{
    // First call father serialization
    Device::serialize(doc);
    EventHandler::serialize(doc);

    doc["pin0"] = pins[0];
    doc["pin1"] = pins[1];
    doc["pin2"] = pins[2];
    doc["pin3"] = pins[3];


    doc["totalSteps"] = totalSteps;
    doc["stepsPerAction"] = stepsPerAction;
    doc["speedRpm"] = speedRpm;
}

void StepperMotor::deserialize(
    JsonObject& doc)
{
   // First call father deserialization
    Device::deserialize(doc);
    EventHandler::deserialize(doc);

    pins[0] = doc["pin0"];
    pins[1] = doc["pin1"];
    pins[2] = doc["pin2"];
    pins[3] = doc["pin3"];


    totalSteps = doc["totalSteps"];
    stepsPerAction = doc["stepsPerAction"];
    speedRpm = doc["speedRpm"];

    // Setup the servo library to the selected pin.
    stepper = Stepper(totalSteps, pins[0], pins[1], pins[2], pins[3]);
    stepper.setSpeed(speedRpm);

    // Connect the stepper library to the selected pins.
    Serial.print("Setting stepper: ");
    Serial.print(totalSteps);
    Serial.print(" pins: ");
    Serial.print(pins[0]);
    Serial.print(", ");
    Serial.print(pins[1]);
    Serial.print(", ");
    Serial.print(pins[2]);
    Serial.print(", ");
    Serial.print(pins[3]);
    Serial.println();
}
