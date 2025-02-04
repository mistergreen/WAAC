/*
  StepperMotor
*/

// Andrea Villa
// villa.andrea@gmail.com

#include <cmath>

#include "StepperMotor.h"
#include "DeviceDelegate.h"
#include "Stepper.h"

StepperMotor::StepperMotor() : Device(), EventHandler()
{
    totalSteps = 0;
    stepsDivision = 0;
    speedRpm = 0;
    performedTriggers = 0;
    performedSteps = 0;
    
    stepper = Stepper();
}

StepperMotor::StepperMotor(char *in_name, 
                           int in_pin0, 
                           int in_pin1, 
                           int in_pin2, 
                           int in_pin3, 
                           int in_dependent_device_id, 
                           int total_steps, 
                           int steps_division, 
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
    stepsDivision = steps_division;
    speedRpm = speed_rpm;
    performedTriggers = 0;
    performedSteps = 0;
    
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
    return stepsDivision;
    
}

void StepperMotor::setSteps(int steps_divison) {
    stepsDivision = steps_divison;
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


void StepperMotor::reset()
{
    performedTriggers = 0;
    performedSteps = 0;
}

void StepperMotor::oneLoop()
{
    // Trigger the servo motor.
    Serial.println("switching on");
    Serial.print("Setting steps: 16");
    stepper.step(16);
}

	
void StepperMotor::switchOn()
{
    Serial.println("StepperMotor::switchOn switching on");

    if (performedTriggers < abs(stepsDivision))
    {
        int stepsToPerform = trunc(totalSteps / stepsDivision);
        float totalNotTruncatedSteps = (totalSteps / stepsDivision) * performedTriggers;

        if ((totalNotTruncatedSteps -  abs(performedSteps)) > 0,5)
        {
            if (stepsDivision > 0)
                stepsToPerform++;
            else
                stepsToPerform--;
        }

        // Trigger the servo motor.
        Serial.println("switching on");
        Serial.print("Setting steps: ");
        Serial.println(stepsToPerform);

        // Adjust the single step
        stepper.step(stepsToPerform + 1);
              
        performedTriggers++;
        performedSteps += stepsToPerform;
        delay(500);
        Serial.print("Performed trigger ");
        Serial.println(performedTriggers);
    }
    else
    {
       Serial.print("Maximum number of setps ");
       Serial.print(performedTriggers);
       Serial.println(" reached");
    }
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
    doc["stepsDivision"] = stepsDivision;
    doc["speedRpm"] = speedRpm;
    doc["performedTriggers"] = performedTriggers;
    doc["performedSteps"] = performedSteps;
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
    speedRpm = doc["speedRpm"];

    if(doc.containsKey("stepsDivision"))
        stepsDivision = doc["stepsDivision"];  
        
    if(doc.containsKey("performedTriggers"))
        performedTriggers = doc["performedTriggers"];
    if(doc.containsKey("performedSteps"))
        performedSteps = doc["performedSteps"];
 
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
