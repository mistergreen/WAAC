/*
  AdaFruitAdaFruitPWM4
*/
#define UNSET -1

#include "Arduino.h"
#include "AdaFruitPWM4.h"
#include "../Adafruit_PWMServoDriver/Adafruit_PWMServoDriver.h"
#include "../Device/Device.h" 
#include "../DeviceDelegate/DeviceDelegate.h"
#include <Time.h>


AdaFruitPWM4::AdaFruitPWM4(char *in_name, int in_dependent_device_id) : Device()
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
    strcpy(classType,"AdaFruitPWM4");
    
    isDay = false; // isDay is the day an event is to occur
    timedIndexCounter = 0;
    
    currentRed = initRed = 0;
    currentGreen = initGreen = 0;
    currentBlue = initBlue = 0;
    currentWhite = initWhite = 0;
    pin = 0;
    oneTime = false;
    
    // setup adafruit obj
    pwmObj = new Adafruit_PWMServoDriver();
    pwmObj->begin();
    pwmObj->setPWMFreq(1600);
    
    
  
}

AdaFruitPWM4::~AdaFruitPWM4() {
    delete pwmObj;
    pwmObj = NULL;
    
}


void AdaFruitPWM4::setPins(int red, int green, int blue, int white) {
    //turn off any previous pin
    switchOff();
    
    redPin = red;
    greenPin = green;
    bluePin = blue;
    whitePin = white;

    
}

void AdaFruitPWM4::getPins(int *inArray) {
    inArray[0] = redPin;
    inArray[1] = greenPin;
    inArray[2] = bluePin;
    inArray[3] = whitePin;
}

void AdaFruitPWM4::loop()
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
                        
                        redStartTime = greenStartTime = blueStartTime = whiteStartTime = initMillis = millis();

                        
                        //in case no duration
                        if(eventTime == durationTime) {
                            pwmObj->setPWM(redPin, 0, red[i]);
                            pwmObj->setPWM(greenPin, 0, green[i]);
                            pwmObj->setPWM(bluePin, 0, blue[i]);
                            pwmObj->setPWM(whitePin, 0, white[i]);
                            oneTime = false;
                        }
                        
                        fadeSpan = (durationTime - eventTime) * 1000L;
                       
                        
                    }
                       
                    if(currentTime >= eventTime && currentTime < durationTime && oneTime == true) {
                        
                        long currentMillis = millis();
                        float percent = (float)(currentMillis-initMillis)/(float)fadeSpan;
                        
                        if(redPin > UNSET) {
                            int redDif = red[i] - initRed; // can be + -
                            long redInterval = fadeSpan/abs(redDif);
                        
                            if(currentMillis - redStartTime >= redInterval) {
                                
                                if(percent >= 0.0f && percent <= 1.0f) {
                                    currentRed = initRed + redDif * percent;
                                }
                                
                                pwmObj->setPWM(redPin, 0, currentRed);
                                redStartTime += redInterval;
                            }
                            
                        }
                        
                        if(greenPin > UNSET) {
                            int greenDif = green[i] - initGreen; // can be + -
                            long greenInterval = fadeSpan/abs(greenDif);
                            
                            if(currentMillis - greenStartTime >= greenInterval) {
                                
                                if(percent >= 0.0f && percent <= 1.0f) {
                                    currentGreen = initGreen + greenDif * percent;
                                }
                                
                                pwmObj->setPWM(greenPin, 0, currentGreen);
                                
                                greenStartTime += greenInterval;
                            }
                        }
             
                        if(bluePin > UNSET) {
                            int blueDif = blue[i] - initBlue; // can be + -
                            long blueInterval = fadeSpan/abs(blueDif);
                            
                            if(currentMillis - blueStartTime >= blueInterval) {
                                
                                if(percent >= 0.0f && percent <= 1.0f) {
                                    currentBlue = initBlue + blueDif * percent;
                                }
                                
                                pwmObj->setPWM(bluePin, 0, currentBlue);
                                
                                blueStartTime += blueInterval;
                            }
                            
                          /*
                             Serial.print(currentMillis);
                             Serial.print(":");
                             Serial.print(initMillis);
                             Serial.print(":");
                             Serial.print(currentMillis-initMillis);
                             Serial.print(":");
                             Serial.print(fadeSpan);
                             Serial.print(":");
                             Serial.print(percent);
                             Serial.print(":");
                             Serial.println(currentBlue);
                            */

                            
                        }
                        
                        if(whitePin > UNSET) {
                            int whiteDif = white[i] - initWhite; // can be + -
                            long whiteInterval = fadeSpan/abs(whiteDif);
                            
                            if(currentMillis - whiteStartTime >= whiteInterval) {
                                
                                if(percent >= 0.0f && percent <= 1.0f) {
                                    currentWhite = initWhite + whiteDif * percent;
                                }
                                
                                pwmObj->setPWM(whitePin, 0, currentWhite);
                                
                                whiteStartTime += whiteInterval;
                                
                            }
                        }
      

                    } else if(currentTime == durationTime && oneTime == true) {
                        //reset and initialize for next event
                        oneTime = false;
                        
                        if(redPin > UNSET) {
                            currentRed = red[i];
                            initRed = currentRed;
                            pwmObj->setPWM(redPin, 0, currentRed);
                        }
                        if(greenPin > UNSET) {
                            currentGreen = green[i];
                            initGreen = currentGreen;
                            pwmObj->setPWM(greenPin, 0, currentGreen);
                        }
                        if(bluePin > UNSET) {
                            currentBlue = blue[i];
                            initBlue = currentBlue;
                            pwmObj->setPWM(bluePin, 0, currentBlue);
                        }
                        if(whitePin > UNSET) {
                            currentWhite = white[i];
                            initWhite = currentWhite;
                            pwmObj->setPWM(whitePin, 0, currentWhite);
                        }
                        
                    }
                    
                }// end dependentDevice
            }//end isDay
        }// end for loop
    }// end suspendTime
}


void AdaFruitPWM4::setEvent(char *in_string)
{

    
    // start time, duration, pwm
    // "08:00:00,01:00:00,255:255:255:255"
    //you can only add up to 4 events- each event is -on and duration -off pairs

    char events[12][16];
    
    //Serial.println(in_string);
    
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

        sscanf(events[j], "%d:%d:%d", &hour[l],&minute[l],&second[l]);
        j++;
        
        sscanf(events[j], "%d:%d:%d", &hourDuration[l],&minuteDuration[l],&secondDuration[l]);
        j++;
        
        //Serial.println(events[j]);
        
        sscanf(events[j], "%d:%d:%d:%d", &red[l],&green[l],&blue[l],&white[l]);
        j++;
        
    }
    

}


void AdaFruitPWM4::getEvent(char *string) {

    if(timedIndexCounter > 0) {
        
        char buf[41];
        
        strcpy(string, dow);
        
        for (uint8_t i=0; i < timedIndexCounter; i++) {
            sprintf(buf, ",%02d:%02d:%02d,%02d:%02d:%02d,%d:%d:%d:%d", hour[i],minute[i],second[i],hourDuration[i],minuteDuration[i],secondDuration[i],red[i],green[i],blue[i],white[i]);
            strcat(string,buf);
        }
        
    }
    
}

int AdaFruitPWM4::getDependentDevice() {
    return dependentDeviceId;
    
}

void AdaFruitPWM4::setDependentDevice(int id) {
    dependentDeviceId = id;
    if(dependentDeviceId) {
        dependentDeviceObject = deviceDelegate.findDevice(dependentDeviceId);
    } else {
        dependentDeviceObject = NULL;
    }

    
}

void AdaFruitPWM4::setSuspendTime(boolean in_suspend) {
    suspendTime = in_suspend;
    
    if(redPin > UNSET) pwmObj->setPWM(redPin, 0, currentRed);
    if(greenPin > UNSET) pwmObj->setPWM(greenPin, 0, currentGreen);
    if(bluePin > UNSET) pwmObj->setPWM(bluePin, 0, currentBlue);
    if(whitePin > UNSET) pwmObj->setPWM(whitePin, 0, currentWhite);
    
}



void AdaFruitPWM4::switchOn()
{
    // address is defined in the device cpp file
    //Serial.println("switching on");
   
    if(redPin > UNSET) pwmObj->setPWM(redPin, 0, 4095);
    if(greenPin > UNSET) pwmObj->setPWM(greenPin, 0, 4095);
    if(bluePin > UNSET) pwmObj->setPWM(bluePin, 0, 4095);
    if(whitePin > UNSET) pwmObj->setPWM(whitePin, 0, 4095);
    
    deviceState = true;
}

void AdaFruitPWM4::switchOff()
{
    //Serial.println("switching off");
    
    if(redPin > UNSET) pwmObj->setPWM(redPin, 0, 0);
    if(greenPin > UNSET) pwmObj->setPWM(greenPin, 0, 0);
    if(bluePin > UNSET) pwmObj->setPWM(bluePin, 0, 0);
    if(whitePin > UNSET) pwmObj->setPWM(whitePin, 0, 0);

    deviceState = false;
}

void AdaFruitPWM4::toggleState() {
    if (deviceState) {
        switchOff();
        
    } else {
        switchOn();
    }
}


void AdaFruitPWM4::setPWMs(int in_red, int in_green, int in_blue, int in_white)
{
    if(redPin > UNSET) pwmObj->setPWM(redPin, 0, in_red);
    if(greenPin > UNSET) pwmObj->setPWM(greenPin, 0, in_green);
    if(bluePin > UNSET) pwmObj->setPWM(bluePin, 0, in_blue);
    if(whitePin > UNSET) pwmObj->setPWM(whitePin, 0, in_white);
    //Serial.println("set pwms");
    //Serial.println(in_red);
}


