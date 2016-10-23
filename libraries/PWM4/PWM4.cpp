/*
  PWM
*/

#include "Arduino.h"
#include "PWM4.h"
#include "../Device/Device.h" 
#include "../DeviceDelegate/DeviceDelegate.h"
#include <Time.h>


PWM4::PWM4(char *in_name, int in_dependent_device_id) : Device()
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
    strcpy(classType,"PWM4");
    
    isDay = false; // isDay is the day an event is to occur
    timedIndexCounter = 0;
    
    currentRed = initRed = 0;
    currentGreen = initGreen = 0;
    currentBlue = initBlue = 0;
    currentWhite = initWhite = 0;
    pin = 0;
    oneTime = false;
  
}


void PWM4::setPins(uint8_t red, uint8_t green, uint8_t blue, uint8_t white) {
    //turn off any previous pin
    switchOff();
    
    redPin = red;
    greenPin = green;
    bluePin = blue;
    whitePin = white;
    
    if(redPin != 0) pinMode(redPin, OUTPUT);
    if(greenPin != 0) pinMode(greenPin, OUTPUT);
    if(bluePin != 0) pinMode(bluePin, OUTPUT);
    if(whitePin != 0) pinMode(whitePin, OUTPUT);

    
}

void PWM4::getPins(uint8_t *inArray) {
    inArray[0] = redPin;
    inArray[1] = greenPin;
    inArray[2] = bluePin;
    inArray[3] = whitePin;
   
}

void PWM4::loop()
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
                    if(currentRed > 0 || currentGreen > 0 || currentBlue > 0 || currentWhite > 0) {
                        deviceState = true;
                    } else {
                        deviceState = false;
                    }
                    
                    if(currentTime == eventTime && oneTime == false) {
                        oneTime = true;
                        initMillis = millis();
                        redStartTime = greenStartTime = blueStartTime = whiteStartTime = initMillis;
                        //in case no duration
                        if(eventTime == durationTime) {
                            analogWrite(redPin, red[i]);
                            analogWrite(greenPin, green[i]);
                            analogWrite(bluePin, blue[i]);
                            analogWrite(whitePin, white[i]);

                            oneTime = false;
                        }
                        
                        

                    }
                       
                    if(currentTime >= eventTime && currentTime < durationTime && oneTime == true) {
                        long fadeSpan = (durationTime - eventTime) * 1000L;
                        long currentMillis = millis();
                        float percent = (float)(currentMillis-initMillis)/(float)fadeSpan;
                        
                        if(redPin != 0) {
                            int redDif = red[i] - initRed; // can be + -
                            long redInterval = fadeSpan/abs(redDif);
                        
                            if(currentMillis - redStartTime >= redInterval) {
                               
                                if(percent >= 0.0f && percent <= 1.0f) {
                                    currentRed = initRed + redDif * percent;
                                }
                                analogWrite(redPin, currentRed);
                                
                                redStartTime += redInterval;
                            }
                        }
                        
                        if(greenPin != 0) {
                            int greenDif = green[i] - initGreen; // can be + -
                            long greenInterval = fadeSpan/abs(greenDif);
                            
                            if(currentMillis - greenStartTime >= greenInterval) {
                                if(percent >= 0.0f && percent <= 1.0f) {
                                    currentGreen = initGreen + greenDif * percent;
                                }
                                analogWrite(greenPin, currentGreen);
                                
                                greenStartTime += greenInterval;
                            }
                        }
             
                        if(bluePin != 0) {
                            int blueDif = blue[i] - initBlue; // can be + -
                            long blueInterval = fadeSpan/abs(blueDif);
                            
                            if(currentMillis - blueStartTime >= blueInterval) {
                                if(percent >= 0.0f && percent <= 1.0f) {
                                    currentBlue = initBlue + blueDif * percent;
                                }

                                analogWrite(bluePin, currentBlue);
                                
                                blueStartTime += blueInterval;
                            }
                        }
                        
                        if(whitePin != 0) {
                            int whiteDif = white[i] - initWhite; // can be + -
                            long whiteInterval = fadeSpan/abs(whiteDif);
                            
                            if(currentMillis - whiteStartTime >= whiteInterval) {
                                if(percent >= 0.0f && percent <= 1.0f) {
                                    currentWhite = initWhite + whiteDif * percent;
                                }

                                analogWrite(whitePin, currentWhite);
                                
                                whiteStartTime += whiteInterval;
                                
                            }
                        }
      

                    } else if(currentTime == durationTime && oneTime == true) {
                        //reset and initialize for next event
                        oneTime = false;
                        if(redPin != 0) {
                            currentRed = red[i];
                            initRed = currentRed;
                            analogWrite(redPin, currentRed);
                        }
                        if(greenPin != 0) {
                            currentGreen = green[i];
                            initGreen = currentGreen;
                            analogWrite(greenPin, currentGreen);
                        }
                        if(bluePin != 0) {
                            currentBlue = blue[i];
                            initBlue = currentBlue;
                            analogWrite(bluePin, currentBlue);
                        }
                        if(whitePin != 0) {
                            currentWhite = white[i];
                            initWhite = currentWhite;
                            analogWrite(whitePin, currentWhite);
                        }
                        
                    }
                    
                }// end dependentDevice
            }//end isDay
        }// end for loop
    }// end suspendTime
}


void PWM4::setEvent(char *in_string)
{

    
    // start time, duration, pwm
    // "08:00:00,01:00:00,255:255:255:255"
    //you can only add up to 4 events- each event is -on and duration -off pairs

    char events[12][16];
    
    Serial.println(in_string);
    
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
    //first set of pwm not getting saved
    for (int l=0; l < timedIndexCounter; l++) {
        Serial.print("lllllll: ");
        Serial.println(l);
        Serial.print("hour: ");
        Serial.println(events[j]);
        
        sscanf(events[j], "%d:%d:%d", &hour[l],&minute[l],&second[l]);
        j++;
        Serial.print("duration: ");
        Serial.println(events[j]);
        sscanf(events[j], "%d:%d:%d", &hourDuration[l],&minuteDuration[l],&secondDuration[l]);
        j++;
        Serial.print("pwm: ");
        Serial.println(events[j]);
        
        sscanf(events[j], "%d:%d:%d:%d", &red[l],&green[l],&blue[l],&white[l]);
        j++;
        
    }
    Serial.print("mmmmmmmm ");
    Serial.print(blue[0]);
    Serial.print(":");
    Serial.println(white[0]);
    

}


void PWM4::getEvent(char *string) {

    if(timedIndexCounter > 0) {
        
        char buf[41];
        
        strcpy(string, dow);
        
        for (uint8_t i=0; i < timedIndexCounter; i++) {
            sprintf(buf, ",%02d:%02d:%02d,%02d:%02d:%02d,%d:%d:%d:%d", hour[i],minute[i],second[i],hourDuration[i],minuteDuration[i],secondDuration[i],red[i],green[i],blue[i],white[i]);
            strcat(string,buf);
        }
        
    }
    
}

int PWM4::getDependentDevice() {
    return dependentDeviceId;
    
}

void PWM4::setDependentDevice(int id) {
    dependentDeviceId = id;
    if(dependentDeviceId) {
        dependentDeviceObject = deviceDelegate.findDevice(dependentDeviceId);
    } else {
        dependentDeviceObject = NULL;
    }

    
}

void PWM4::setSuspendTime(boolean in_suspend) {
    suspendTime = in_suspend;
    
    if(redPin != 0) analogWrite(redPin, currentRed);
    if(greenPin != 0) analogWrite(greenPin, currentGreen);
    if(bluePin != 0) analogWrite(bluePin, currentBlue);
    if(whitePin != 0) analogWrite(whitePin, currentWhite);
    
}



void PWM4::switchOn()
{
    // address is defined in the device cpp file
    //Serial.println("switching on");
   
    if(redPin != 0) analogWrite(redPin,255);
    if(greenPin != 0) analogWrite(greenPin,255);
    if(bluePin != 0) analogWrite(bluePin,255);
    if(whitePin != 0) analogWrite(whitePin,255);
    
    deviceState = true;
}

void PWM4::switchOff()
{
    //Serial.println("switching off");
    
    if(redPin != 0) analogWrite(redPin,0);
    if(greenPin != 0) analogWrite(greenPin,0);
    if(bluePin != 0) analogWrite(bluePin,0);
    if(whitePin != 0) analogWrite(whitePin,0);

    deviceState = false;
}

void PWM4::toggleState() {
    if (deviceState) {
        switchOff();
        
    } else {
        switchOn();
    }
}

void PWM4::setPWMs(uint8_t in_red, uint8_t in_green, uint8_t in_blue, uint8_t in_white)
{
    if(redPin != 0) analogWrite(redPin, in_red);
    if(greenPin != 0) analogWrite(greenPin, in_green);
    if(bluePin != 0) analogWrite(bluePin, in_blue);
    if(whitePin != 0) analogWrite(whitePin, in_white);
    
}


