/*
  PWM
*/

#include "Arduino.h"
#include "AdaFruitPWM.h"
#include "../Adafruit_PWMServoDriver/Adafruit_PWMServoDriver.h"
#include "../Device/Device.h" 
#include "../DeviceDelegate/DeviceDelegate.h"
#include <Time.h>


AdaFruitPWM::AdaFruitPWM(char *in_name, int in_pin, int in_dependent_device_id) : Device()
{
    // Device() call super to init vars
    dependentDeviceId = in_dependent_device_id;
    //find dependent device once - not all the time
    if(dependentDeviceId) {
        dependentDeviceObject = deviceDelegate.findDevice(dependentDeviceId);
    }
    
    //deviceID is automatically set my deviceDeleGate

    strcpy(deviceName,in_name);
    //classType inherit from base
    strcpy(classType,"AdaFruitPWM");

    pin = in_pin; // channel to adaFruitPWM
    
    isDay = false; // isDay is the day an event is to occur
    timedIndexCounter = 0;
    startTime = 0UL;
    currentPWM = 0;
    initPWM = 0;
    oneTime = false;
    
    // setup adafruit obj
    pwmObj = new Adafruit_PWMServoDriver();
    pwmObj->begin();
    pwmObj->setPWMFreq(1600);
}

AdaFruitPWM::~AdaFruitPWM() {
    delete pwmObj;
    pwmObj = NULL;
    
}


void AdaFruitPWM::loop()
{
    if(suspendTime == false) {
        // when no time events
        if(timedIndexCounter == 0) {
            if(dependentDeviceObject) { //if there's a dependent device
                //find dependent device - call DeviceDelegate
                boolean temp = dependentDeviceObject->getDeviceState();
                
                if(temp) {
                    switchOn();
                } else {
                    switchOff();
                }
            }
            
        }
        
        //loop through events & check for time
        for (uint8_t i=0; i < timedIndexCounter; i++) {
            //dow comming in '1111110' 7 places 0 - sunday
            // isDay is the day an event is to occur
            //Time.h sunday is 1
            if(dow[::weekday()-1] == '1') {
                isDay = true;
                //Serial.println("isDay of event");
            } else {
                isDay = false;
                //Serial.println("notDay!");
            }
            
            
            
            if(isDay) {
                //::hour() get time from Time.h
                long currentTime = convertToSeconds(::hour(),::minute(),::second());
                //get start time
                long eventTime = convertToSeconds(hour[i],minute[i],second[i]);
                long durationTime = convertToSeconds(hourDuration[i],minuteDuration[i],secondDuration[i]) + convertToSeconds(hour[i],minute[i],second[i]);
                //rollover midnight
                if (durationTime > 86400L) {
                    durationTime = durationTime - 86400L;
                }
                
                if(dependentDeviceObject) {
                    //if there's a dependent device
                    //do nothing
                } else {
                    // if no dependent
                    if(currentPWM > 0) {
                        deviceState = true;
                    } else {
                        deviceState = false;
                    }
                    
                    if(currentTime == eventTime && oneTime == false) {
          
                        oneTime = true;
                        startTime = initMillis = millis();
                        //in case no duration
                        if(eventTime == durationTime) {
                            pwmObj->setPWM(pin, 0, pwmValue[i]);
                
                            oneTime = false;
                        }
                        // compensate for quick transitions (durationTime-1)
                        fadeSpan = (durationTime - eventTime) * 1000L;
                        pwmDif = pwmValue[i] - initPWM; // can be + -
                        interval = fadeSpan/abs(pwmDif); // in ms
                       
                    }
                    
                    if(currentTime >= eventTime && currentTime < durationTime && oneTime == true) {
                            long currentMillis = millis();
                            if(currentMillis - startTime >= interval) {
                                
                                float percent = (float)(currentMillis-initMillis)/(float)fadeSpan;
                                
                                if(percent >= 0.0f && percent <= 1.0f) {
                                    currentPWM = initPWM + pwmDif * percent;
                                }
                                
                                pwmObj->setPWM(pin, 0, currentPWM);
                                
                                startTime += interval;
                                
                            }
                        
                        
                    } else if(currentTime == durationTime && oneTime == true) {
                        //reset and initialize for next event
                        //Serial.println("time stop");
                        oneTime = false;
                        initPWM = pwmValue[i];
                        pwmObj->setPWM(pin, 0, initPWM);
                        
                    }

                    
                    
                }// end dependentDevice
                
                
            }//end isDay
        }//end for loop
    }//end suspendtime
}


void AdaFruitPWM::setEvent(char *in_string)
{

    
    // start time, duration, pwm
    // "8:00:00,1:00:00,255"
    //you can only add up to 4 events- each event is -on and duration -off pairs
    //    [3*4 events] [ch per event]
    char events[12][9];
    
    
    //parse incoming string *** MAKE ROOM FOR THE NUL TERMINATOR in the string!
    char *tok1;
    int i = 0;
    int k = 0;
    tok1 = strtok(in_string, ",");
    while (tok1 != NULL) {
        if(i != 0) {
            strcpy(events[k],tok1);
            k++;
        } else {
            //strip out the first param as dow
            strcpy(dow,tok1);
        }
        tok1 = strtok(NULL, ",");
        i++;
    }
    
    int j = 0;
    timedIndexCounter = i/3;
    
    
    for (int l=0; l < i/3; l++) {
        sscanf(events[j], "%d:%d:%d", &hour[l],&minute[l],&second[l]);
        j++;
        sscanf(events[j], "%d:%d:%d", &hourDuration[l],&minuteDuration[l],&secondDuration[l]);
        j++;
        pwmValue[l] = atoi(events[j]);
        j++;
        
    }

}


void AdaFruitPWM::getEvent(char *string) {

    if(timedIndexCounter > 0) {
        char buf[27];
        
        strcpy(string, dow);
        
        for (uint8_t i=0; i < timedIndexCounter; i++) {
            sprintf(buf, ",%02d:%02d:%02d,%02d:%02d:%02d,%d", hour[i],minute[i],second[i],hourDuration[i],minuteDuration[i],secondDuration[i],pwmValue[i]);
            strcat(string,buf);
        }
        
    }
    
}

int AdaFruitPWM::getDependentDevice() {
    return dependentDeviceId;
    
}

void AdaFruitPWM::setDependentDevice(int id) {
    dependentDeviceId = id;
    if(dependentDeviceId) {
        dependentDeviceObject = deviceDelegate.findDevice(dependentDeviceId);
    } else {
        dependentDeviceObject = NULL;
    }

    
}

void AdaFruitPWM::setSuspendTime(boolean in_suspend) {
    suspendTime = in_suspend;
    pwmObj->setPWM(pin, 0, currentPWM);
   
}


void AdaFruitPWM::switchOn()
{
    // address is defined in the device cpp file
    //Serial.println("switching on");
   
    pwmObj->setPWM(pin, 0, 4095);
    deviceState = true;
}

void AdaFruitPWM::switchOff()
{
    // address is defined in the device cpp file
    //Serial.println("switching off");
    
    pwmObj->setPWM(pin, 0, 0);
    deviceState = false;
}

void AdaFruitPWM::toggleState() {
    if (deviceState) {
        switchOff();
        
    } else {
        switchOn();
    }
}

void AdaFruitPWM::setPWM(int in_pwm)
{
    
    pwmObj->setPWM(pin, 0, in_pwm);
}


