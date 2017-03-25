/*
  AdaFruitAdaFruitPWM8
*/

// © 2noodles llc
// minh@2noodles.com

#define UNSET -1


#include "Arduino.h"
#include "AdaFruitPWM8.h"
#include <Wire.h>
#include "../Adafruit_PWMServoDriver/Adafruit_PWMServoDriver.h"
#include "../Device/Device.h" 
#include "../DeviceDelegate/DeviceDelegate.h"
#include <Time.h>


AdaFruitPWM8::AdaFruitPWM8(char *in_name, int in_dependent_device_id) : Device()
{
    // Device() call super to init vars
    dependentDeviceId = in_dependent_device_id;
    //find dependent device once - not all the time
    setDependentDevice(dependentDeviceId);
    
    //deviceID is automatically set my deviceDeleGate

    strcpy(deviceName,in_name);
    //classType inherit from base
    strcpy(classType,"AdaFruitPWM8");
    
    isDay = false; // isDay is the day an event is to occur
    timedIndexCounter = 0;
    
    for(int i=0; i < CHANNEL8;i++) {
        color[i].currentColor = 0;
        color[i].initColor = 0;
    }
    pin = 0;
    oneTime = false;
    
    // setup adafruit obj
   // pwmObj = new Adafruit_PWMServoDriver();
    pwmObj = Adafruit_PWMServoDriver();
    pwmObj.begin();
    pwmObj.setPWMFreq(1600);
    
    hasEvent = false;
  
}


AdaFruitPWM8::~AdaFruitPWM8() {
    //clean up
    switchOff();
    
    //delete pwmObj;
    //pwmObj = NULL; // dont need too. NULL to pointer
    
    //need clean up the struct?
    //colorAux = NULL;
}


void AdaFruitPWM8::setHasEvent(bool in_hasevent) {
    hasEvent = in_hasevent;
}


void AdaFruitPWM8::setPins(int red, int green, int blue, int white, int aux1, int aux2, int aux3, int aux4) {
    //turn off any previous pin
    //switchOff();
    
    color[0].pin = red;
    color[1].pin = green;
    color[2].pin = blue;
    color[3].pin = white;
    color[4].pin = aux1;
    color[5].pin = aux2;
    color[6].pin = aux3;
    color[7].pin = aux4;
    //Serial.println("pins saved");
   
}

void AdaFruitPWM8::getPins(int *inArray) {
    
     for(int i=0; i < CHANNEL8; i++) {
         inArray[i] = color[i].pin;
     }
    
}

void AdaFruitPWM8::loop()
{
    //Serial.println(minuteDuration[0]);
    
    if(suspendTime == false) {
        //Serial.println("has events******");
        //Serial.println(hasEvent);
        //***************************** when no time events
        if(timedIndexCounter == 0) {
            if(dependentDeviceId > 0) { //if there's a dependent device
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
                
                
                if(dependentDeviceId > 0) {
//Serial.println("yes dependat");
//Serial.println(dependentDeviceId);
                    //dependentDeviceObject - can't test a reference
                    //if there's a dependent device
                    //do nothing for now, maybe suspend/enable lights?
                    boolean temp = dependentDeviceObject->getDeviceState();
                    
                    if(temp) {
                        switchOn();
                    } else {
                        switchOff();
                    }

                    
                } else {
//Serial.println("no dependat");
                    
                    // if no dependent
                    deviceState = false;
                    
                    for(uint8_t k=0; k < CHANNEL8; k++) {
                        if(color[k].currentColor > 0) {
                            deviceState = true;
                            break;
                        }
                    }

                    
                    if(currentTime == eventTime && oneTime == false) {
                        
//Serial.println("init fade******");
// Serial.println(currentTime);
// Serial.println(eventTime);
                        // init fading
                        oneTime = true;
                        
                        initMillis = millis();
                        
                        for(uint8_t k=0; k < CHANNEL8; k++) {
                            color[k].colorStartTime = initMillis;
                        }

                        //in case no duration
                        if(eventTime == durationTime) {
                            
                            for(uint8_t k=0; k < CHANNEL8; k++) {
                                pwmObj.setPin(color[k].pin, color[k].pwm[i], false);
                            }
                            
                            
                            oneTime = false;
                        }
                        
                        fadeSpan = (durationTime - eventTime) * 1000L;
                        
                    }
                    
                     //fading colors
                    if(currentTime >= eventTime && currentTime < durationTime && oneTime == true) {
 //Serial.println("start fading******");
 
                        long currentMillis = millis();
                        float percent = (float)(currentMillis-initMillis)/(float)fadeSpan;
                        
                        for(uint8_t k=0; k < CHANNEL8; k++) {
                            if(color[k].pin > UNSET) {
                                int colorDif = color[k].pwm[i] - color[k].initColor; // can be + -
                                long colorInterval = fadeSpan/abs(colorDif);
                                
                                if(currentMillis - color[k].colorStartTime >= colorInterval) {
                                    
                                    if(percent >= 0.0f && percent <= 1.0f) {
                                        color[k].currentColor = color[k].initColor + colorDif * percent;
                                    }
                                    
                                    //Serial.println("****** pin 0 - ");
                                    //Serial.println(i);
                                    //Serial.println(color[0].currentColor);
                                    
                                    //pwmObj.setPWM(color[k].pin, 0, color[k].currentColor);
                                    pwmObj.setPin(color[k].pin, color[k].currentColor, false);
                                    color[k].colorStartTime += colorInterval;
                                }
                                
                            }

                        }

                    } else if(currentTime == durationTime && oneTime == true) {
                        //reset and initialize for next event
                        oneTime = false;
      //Serial.println("end fading******");
                        for(uint8_t k=0; k < CHANNEL8; k++) {
                            if(color[k].pin > UNSET) {
                                color[k].currentColor = color[k].pwm[i];
                                color[k].initColor = color[k].currentColor;
                                pwmObj.setPin(color[k].pin, color[k].currentColor, false);
     //Serial.println(i);
     //Serial.println(color[k].currentColor);
                            }
                            
                        }
                        
                    }
                    
                }// end dependentDevice
            }//end isDay
        }// end for loop
    }// end suspendTime
}


void AdaFruitPWM8::setEvent(char *in_string)
{
    //Serial.println("++++++in event+++++++");
    //Serial.println(in_string);
    
    if(in_string[0] == '\0') return;
    // start time, duration, pwm
    // "08:00:00,01:00:00,1255:1255:1255:155:1255:1255:1255:1255"
    //you can only add up to 4 events- each event is -on and duration -off pairs

    char events[12][58];
    
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
    
    // start time, duration, pwm
    timedIndexCounter = i/3;
    //first set of pwm not getting saved
    // either or sscanf or strtok
    for (int l=0; l < timedIndexCounter; l++) {
    
        sscanf(events[j], "%d:%d:%d", &hour[l],&minute[l],&second[l]);
        j++;
        
        sscanf(events[j], "%d:%d:%d", &hourDuration[l],&minuteDuration[l],&secondDuration[l]);
        //Serial.println("++++++in duration+++++++");
        //Serial.println(events[j]);
        //Serial.println(hourDuration[l]);
        //Serial.println(minuteDuration[l]);
        j++;

        
        //Serial.println(events[j]);
        
        sscanf(events[j], "%d:%d:%d:%d:%d:%d:%d:%d", &color[0].pwm[l], &color[1].pwm[l], &color[2].pwm[l], &color[3].pwm[l], &color[4].pwm[l], &color[5].pwm[l], &color[6].pwm[l], &color[7].pwm[l]);
        j++;
       
        
        /*
        int tempArr1[3] = {0};
        stripTime(events[j], tempArr1);
        hour[l] = tempArr1[0];
        minute[l] = tempArr1[1];
        second[l] = tempArr1[2];
        
        
        j++;
        
        int tempArr2[3] = {0};
        stripTime(events[j], tempArr2);
        hourDuration[l] = tempArr2[0];
        minuteDuration[l] = tempArr2[1];
        secondDuration[l] = tempArr2[2];
        j++;
        
        int tempArr3[CHANNEL8] = {0};
        stripTime(events[j], tempArr3);
        for(int k=0; k < CHANNEL8; k++) {
            color[k].pwm[l] = tempArr3[k];
        }
        j++;
        */
        
        
    }
        /*
    Serial.println("********");
    Serial.print(color[4].pwm[0]);
     Serial.print(", ");
    Serial.print(color[5].pwm[0]);
    Serial.print(", ");
    Serial.print(color[6].pwm[0]);
    Serial.print(", ");
    Serial.print(color[7].pwm[0]);
    Serial.println("********");
     */
    
   // Serial.println("!!!!!in duration!!!!");
//!!!!!!!!!!!!! adding more than one event wipes out the first time
    //Serial.println(hourDuration[0]);
    //Serial.println(minuteDuration[0]);
    
    
}

void AdaFruitPWM8::stripTime(char *inString, int *inArray) {
    char *tok1;
    int i = 0;
    tok1 = strtok(inString, ":");
    while (tok1 != NULL) {
        inArray[i] = atoi(tok1);
        tok1 = strtok(NULL, ":");
        i++;
        
    }

    
}



void AdaFruitPWM8::getEvent(char *string) {

    if(timedIndexCounter > 0) {
        
        char buf[240];
        
        strcpy(string, dow);
        
        for (uint8_t i=0; i < timedIndexCounter; i++) {
//*********** hourDuration[0] not saved!
//Serial.println("<<<<<in mnute<<<<<");
//Serial.println(i);
//Serial.println(minute[i]);
//Serial.println(minuteDuration[i]);
            
            sprintf(buf, ",%02d:%02d:%02d,%02d:%02d:%02d,%d:%d:%d:%d:%d:%d:%d:%d", hour[i], minute[i], second[i], hourDuration[i], minuteDuration[i], secondDuration[i], color[0].pwm[i], color[1].pwm[i], color[2].pwm[i], color[3].pwm[i], color[4].pwm[i], color[5].pwm[i], color[6].pwm[i], color[7].pwm[i]);
            strcat(string,buf);
        }
        
    } else {
        string[0] = '\0';
        
    }
    
}

int AdaFruitPWM8::getDependentDevice() {
    return dependentDeviceId;
    
}

void AdaFruitPWM8::setDependentDevice(int id) {
    dependentDeviceId = id;
    if(dependentDeviceId > 0) {
        dependentDeviceObject = deviceDelegate.findDevice(dependentDeviceId);
    } else {
        dependentDeviceObject = NULL;
    }

    
}

void AdaFruitPWM8::setSuspendTime(boolean in_suspend) {
    suspendTime = in_suspend;
    //save current vals
    //reset leds
    for(uint8_t k=0; k < CHANNEL8; k++) {
        if(color[k].pin > UNSET) {
             pwmObj.setPin(color[k].pin, color[k].currentColor, false);
        }
    }
    
    
}



void AdaFruitPWM8::switchOn()
{
    // address is defined in the device cpp file
    //Serial.println("switching on");
   
    for(uint8_t k=0; k <  CHANNEL8; k++) {
        if(color[k].pin > UNSET) {
            pwmObj.setPin(color[k].pin, 4095, false);
        }
    }
    
    
    deviceState = true;
}

void AdaFruitPWM8::switchOff()
{
    //Serial.println("switching off");
    
    for(uint8_t k=0; k < CHANNEL8;k++) {
        if(color[k].pin > UNSET) {
            pwmObj.setPin(color[k].pin, 0, false);
        }
    }

    deviceState = false;
}

void AdaFruitPWM8::toggleState() {
    if (deviceState) {
        switchOff();
        
    } else {
        switchOn();
    }
}


void AdaFruitPWM8::setPWMs(int in_red, int in_green, int in_blue, int in_white, int in_aux1, int in_aux2, int in_aux3, int in_aux4)
{
    int tempColor[8] = {in_red, in_green, in_blue, in_white, in_aux1, in_aux2, in_aux3, in_aux4};
    
    for(uint8_t k=0; k < CHANNEL8; k++) {
        if(color[k].pin > UNSET) {
            pwmObj.setPin(color[k].pin, tempColor[k], false);
            //Serial.println(color[k].pin);
            //Serial.println(tempColor[k]);
            //Serial.println("------");
        }
    }
    
    //Serial.println("set pwms");
    //Serial.println(in_red);
}


