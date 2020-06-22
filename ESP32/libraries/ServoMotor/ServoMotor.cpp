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
#include <TimeLib.h>

ServoMotor::ServoMotor() : Device()
{
    isDay = false; // isDay is the day an event is to occur
    timedIndexCounter = 0;
	stopAngle = 0;
	moveAngle = 0;
	secondDuration = 0;
    
    dependentDeviceObject = NULL;
    
    memset(hour, 0, sizeof(int)*5);
    memset(minute, 0, sizeof(int)*5);
    memset(second, 0, sizeof(int)*5);
    memset(dow, 0, sizeof(char)*5*8);
}

ServoMotor::ServoMotor(char *in_name, int in_pin, int in_dependent_device_id, int stop_angle, int move_angle, int second_duration) : Device()
{
    // Device() call super to init vars
    dependentDeviceId = in_dependent_device_id;
    //find dependent device once - not all the time
    setDependentDevice(dependentDeviceId);
    
    
    //deviceID is automatically set my deviceDeleGate

    strcpy(deviceName,in_name);
    //classType inherit from base
    strcpy(classType,"ServoMotor");

    pin = in_pin;

    isDay = false; // isDay is the day an event is to occur
    timedIndexCounter = 0;
    
    stopAngle = stop_angle;
    moveAngle = move_angle;
    secondDuration = second_duration;
    servingTime = 0;
    
    memset(hour, 0, sizeof(int)*5);
    memset(minute, 0, sizeof(int)*5);
    memset(second, 0, sizeof(int)*5);
    memset(dow, 0, sizeof(char)*5*8);
    
    // Connect the servo library to the selected pin.
    Serial.print("Setting angle: ");
    Serial.println(stopAngle);
    servo.attach(pin);
    servo.write(stopAngle);
}


void ServoMotor::loop()
{
    if(suspendTime == false) {
        // when no time events
        if(timedIndexCounter == 0) {
            if(dependentDeviceObject) { //if there's a dependent device
                //find dependent device - call DeviceDelegate
                boolean temp = dependentDeviceObject->getDeviceState();
                
                if(temp) {
                    Serial.println("Trigger by dependent device");
                    trigger();
                }
            }
            
        }
        
        //loop through events & check for time
        for (uint8_t i=0; i < timedIndexCounter; i++) {
            //dow comming in '1111110' 7 places 0 - sunday
            // isDay is the day an event is to occur
            //Time.h sunday is 1
            if(dow[i][::weekday()-1] == '1') {
                isDay = true;
                //Serial.println("isDay of event");
            } else {
                isDay = false;
                //Serial.println("notDay!");
            }
            
            
            // If this is the trigger day.
            if(isDay) {
                //::hour() get time from Time.h
                long currentTime = convertToSeconds(::hour(),::minute(),::second());
                //get start time
                long eventTime = convertToSeconds(hour[i],minute[i],second[i]);
				
                //if there's a dependent device
                if(dependentDeviceId > 0) { 
                    // Check that it is the time to trigger.
                    if(currentTime == eventTime && servingTime != currentTime)
                    {
                        boolean temp = dependentDeviceObject->getDeviceState();
                        
                        if(temp) {
                            Serial.println("Trigger by time - no dependent device");

							servingTime = currentTime;
							trigger();
                        }
                    }
                    
                }
				// if no dependent
				else {
					// Check that it is the time to trigger.
                    if(currentTime == eventTime && servingTime != currentTime) {
                        Serial.println("Trigger by time - no dependent device");
                        
						servingTime = currentTime;
                        trigger();
                    }
                }
            }//end isDay
            
            yield();
        }// end for loop
    }// end suspend time
}



void ServoMotor::setEvent(char *in_string)
{
    Serial.println("in event");
    //Serial.println(in_string);
    if(in_string[0] == '\0') return;
    // start time, duration, dow
    // "8:00:00,1:00:00,1111111,9:30:00,1:30:01,1101111,15:34:02,1:05:02,1010101"
    //you can only add up to 4 events- each event is -on and duration -off pairs
    Serial.println("out event");
    char events[8][67];
    memset(events, 0, sizeof(char)*8*67);
    
    //parse incoming string *** MAKE ROOM FOR THE NUL TERMINATOR in the string!
    char *tok1;
    int i = 0;
    tok1 = strtok(in_string, ",");
    while (tok1 != NULL) {
        strcpy(events[i],tok1);
        tok1 = strtok(NULL, ",");
        i++;
    }
    
    int j = 0;
    //char *token_s;
    //int temp[3];
    
    timedIndexCounter = uint8_t(i/2);
    Serial.println("timedIndexCounter");
    Serial.println(timedIndexCounter);
    
#ifdef ESP8266
    for (uint8_t l=0; l < timedIndexCounter; l++) {
        
        //start
        
        int tempArr1[3] = {0};
        stripTime(events[j], tempArr1);
        hour[l] = tempArr1[0];
        minute[l] = tempArr1[1];
        second[l] = tempArr1[2];
        j++;  
        
        //dow
        strcpy(dow[l],events[j]);
        j++;
        
        yield();
        
    }
#elif ESP32
    for (uint8_t l=0; l < timedIndexCounter; l++) {

        //start
        
        int tempArr1[3] = {0};
        stripTime(events[j], tempArr1);
        hour[l] = tempArr1[0];
        minute[l] = tempArr1[1];
        second[l] = tempArr1[2];
        j++; 
        
        //dow
        strcpy(dow[l],events[j]);
        j++;
        
        yield();
        
    }
#else
    
    for (uint8_t l=0; l < timedIndexCounter; l++) {
        sscanf(events[j], "%d:%d:%d", &hour[l],&minute[l],&second[l]);
        j++;
        /*sscanf(events[j], "%d:%d:%d", &hourDuration[l],&minuteDuration[l],&secondDuration[l]);
        j++;*/
        strcpy(dow[l],events[j]);
        j++;
        
    }

#endif
    

}


void ServoMotor::getEvent(char *string) {
    //manipulat incoming string rather than returning. Saves memory?
    if(timedIndexCounter > 0) {
        Serial.println("crash with event");
        Serial.println("timedIndexCounter");
        Serial.println(timedIndexCounter);
        
        char buf[104];
        memset(buf, 0, sizeof(char)*104);
    
        sprintf(buf, "%02d:%02d:%02d,%s", hour[0],minute[0],second[0],dow[0]);
        strcpy(string, buf);
        
        for (uint8_t i=1; i < timedIndexCounter; i++) {
            sprintf(buf, ",%02d:%02d:%02d,%s", hour[i],minute[i],second[i],dow[i]);
            strcat(string,buf);
        }
    } else {
        Serial.println("crash no event");
        string[0] = '\0';
    }
}

int ServoMotor::getDependentDevice() {
    return dependentDeviceId;
    
}

void ServoMotor::setDependentDevice(int id) {
    dependentDeviceId = id;
    if(dependentDeviceId > 0) {
        dependentDeviceObject = deviceDelegate.findDevice(dependentDeviceId);
    } else {
        dependentDeviceObject = NULL;
    } 
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
	

void ServoMotor::trigger()
{
    // Trigger the servo motor.
    Serial.println("switching on");
    Serial.print("Setting angle: ");
    Serial.println(moveAngle);
    servo.write(moveAngle);
    delay(secondDuration);
    Serial.print("Setting angle: ");
    Serial.println(stopAngle);
    servo.write(stopAngle);
    servingTime = 0;
}

void ServoMotor::toggleState()
{
    trigger();
}


void ServoMotor::serialize(JsonObject& doc)
{
    // First call father serialization
    Device::serialize(doc);

    doc["stopAngle"] = stopAngle;
    doc["moveAngle"] = moveAngle;
    doc["secondDuration"] = secondDuration;
    doc["servingTime"] = servingTime;
    
    // 64 characters per event + carriage return
    char event[67*4];
    getEvent(event);
    doc["event"] = event;
}

void ServoMotor::deserialize(
    JsonObject& doc)
{
   // First call father deserialization
    Device::deserialize(doc);
    
    stopAngle = doc["stopAngle"];
    moveAngle = doc["moveAngle"];
    secondDuration = doc["secondDuration"];
    servingTime = doc["servingTime"];
    
    // 64 characters per event + carriage return
    char event[67*4];
    strcpy (event, doc["event"]);
    
    setEvent(event);
    
    // Connect the servo library to the selected pin.
    servo.attach(pin);
    
    Serial.print("Setting angle: ");
    Serial.println(stopAngle);
    servo.write(stopAngle);
}
