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

#include "Servo.h"

class ServoMotor : public Device
{
  public:
    ServoMotor();
    ServoMotor(char *in_name, int in_pin, int in_dependent_device_id, int stop_angle, int move_angle, int second_duration);
    ~ServoMotor() {}; // destructor

    void loop(); // required
    virtual void trigger();
    
    void setEvent(char *in_string); // override
    void getEvent(char *string); // override
    
    int getDependentDevice(); // override
    void setDependentDevice(int id); // override

    int getDuration();
    void setDuration(int duration);
    
	uint8_t getStopAngle();
    void setStopAngle(uint8_t angle);
	
	uint8_t getMoveAngle();
    void setMoveAngle(uint8_t angle);	
	
    int dependentDeviceId;
    Device *dependentDeviceObject;
    
  private:
	// The servo motor controller.
	Servo servo;
	uint8_t stopAngle;
	uint8_t moveAngle;
	int secondDuration;
	
    boolean isDay;
    uint8_t timedIndexCounter;
	// The current triggered serving time. It is needed to trigger the servo motor only once.
	long servingTime;
    
	// Events stored timings.
	int hour[5];
    int minute[5];
    int second[5];
    char dow[5][8];
    
};//need ; at the end of a class def

#endif