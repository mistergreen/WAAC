/*
  ServoMotor
*/

// © 2noodles llc
// minh@2noodles.com
// Andrea Villa
// villa.andrea@gmail.com

#include "ServoMotor.h"
#include "DeviceDelegate.h"
#include "Servo.h"

ServoMotor::ServoMotor() : Device(), EventHandler()
{
	stopAngle = 0;
	moveAngle = 0;
	secondDuration = 0;
}

ServoMotor::ServoMotor(char *in_name, int in_pin, int in_dependent_device_id, int stop_angle, int move_angle, int second_duration) : Device(), EventHandler(in_dependent_device_id)
{
    strcpy(deviceName, in_name);
    //classType inherit from base
    strcpy(classType, "ServoMotor");

    pin = in_pin;
    
    stopAngle = stop_angle;
    moveAngle = move_angle;
    secondDuration = second_duration;
    
    // Connect the servo library to the selected pin.
    Serial.print("Setting angle: ");
    Serial.println(stopAngle);
    servo.attach(pin);
    servo.write(stopAngle);
}


void ServoMotor::loop()
{
    EventHandler::loop();
}

void ServoMotor::performActionInEvent()
{
    // Change state only if needed, so only once.
    if (false == deviceState)
    {
        switchOn();

        deviceState = true;
    }
}


void ServoMotor::performActionOutEvent()
{
    deviceState = false;
}


int ServoMotor::getDuration() {
    return secondDuration;
    
}

void ServoMotor::setDuration(int duration) {
    secondDuration = duration;
}

uint8_t ServoMotor::getStopAngle() {
	return stopAngle;
}

void ServoMotor::setStopAngle(uint8_t angle) {
    stopAngle = angle;
    servo.write(stopAngle);
}

uint8_t ServoMotor::getMoveAngle() {
    return moveAngle;
}

void ServoMotor::setMoveAngle(uint8_t angle) {
    moveAngle = angle;
}

void ServoMotor::setPin(int in_pin) {
    Device::setPin(in_pin);
    
    // Connect the servo library to the selected pin.
    servo.attach(pin);
}
	
void ServoMotor::switchOn()
{
    Serial.println("Relay::switchOn switching on");

    // Trigger the servo motor.
    Serial.println("switching on");
    Serial.print("Setting angle: ");
    Serial.println(moveAngle);
    servo.write(moveAngle);
    delay(secondDuration);

    // Set a second time for safety
    delay(secondDuration);
    servo.write(stopAngle);
    Serial.print("Setting angle: ");
    Serial.println(stopAngle);
    servo.write(stopAngle);
}


void ServoMotor::toggleState()
{
    // Send one event only if there is no event in progress
    if (false == deviceState) {
        switchOn();
    }
}


void ServoMotor::serialize(JsonObject& doc)
{
    // First call father serialization
    Device::serialize(doc);
    EventHandler::serialize(doc);

    doc["stopAngle"] = stopAngle;
    doc["moveAngle"] = moveAngle;
    doc["secondDuration"] = secondDuration;
}

void ServoMotor::deserialize(
    JsonObject& doc)
{
   // First call father deserialization
    Device::deserialize(doc);
    EventHandler::deserialize(doc);

    stopAngle = doc["stopAngle"];
    moveAngle = doc["moveAngle"];
    secondDuration = doc["secondDuration"];
    
    // Connect the servo library to the selected pin.
    servo.attach(pin);
    
    Serial.print("Setting angle: ");
    Serial.println(stopAngle);
    servo.write(stopAngle);
}
