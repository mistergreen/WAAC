/*
 // © 2noodles llc
 // minh@2noodles.com
*/

#define UNSET -1

#include "PWM4.h"
#include "DeviceDelegate.h"
#include "TimeLib.h"


PWM4::PWM4(char *in_name, int in_dependent_device_id) : Device()
{
    // Device() call super to init vars
    dependentDeviceId = in_dependent_device_id;
    //find dependent device once - not all the time
    setDependentDevice(dependentDeviceId);
    
    //deviceID is automatically set my deviceDeleGate

    strcpy(deviceName,in_name);
    //classType inherit from base
    strcpy(classType,"PWM4");
    
    isDay = false; // isDay is the day an event is to occur
    timedIndexCounter = 0;
    
    for(int i=0; i < CHANNEL;i++) {
        color[i].currentColor = 0;
        color[i].initColor = 0;
    }
    
    pin = 0;
    oneTime = false;
  
}

PWM4::~PWM4() {
    //clean up
    switchOff();
}

void PWM4::setPins(int red, int green, int blue, int white, int channel0, int channel1, int channel2, int channel3 ) {
    //turn off any previous pin
    switchOff();
    
    color[0].pin = red;
    color[1].pin = green;
    color[2].pin = blue;
    color[3].pin = white;
    
    color[0].pwmChannel = channel0;
    color[1].pwmChannel = channel1;
    color[2].pwmChannel = channel2;
    color[3].pwmChannel = channel3;
    
     // set pinmode in case - overwrite previous settings
    for(uint8_t i = 0; i < CHANNEL; i++) {
        if(color[i].pin > UNSET) pinMode(color[i].pin, OUTPUT);
        
    }

    
    //attachpins to channels - pin/channel pair
    
    for(uint8_t i = 0; i < CHANNEL; i++) {
        if(color[i].pin > UNSET) {
            ledcSetup(color[i].pwmChannel, LEDC_BASE_FREQ, LEDC_TIMER_BIT);
            ledcAttachPin(color[i].pin, color[i].pwmChannel);
        }
        
    }
    //turn lights back on to current state
    //when suspendTime is called
    
}

void PWM4::getPins(int *inPinArray, int *inChannelArray) {
//Serial.println("get PWM4 library pins");
    for(int i=0; i < CHANNEL; i++) {
        inPinArray[i] = color[i].pin;
        inChannelArray[i] = color[i].pwmChannel;
    }
}


void PWM4::loop()
{
    if(suspendTime == false) {
        // when no time events
        if(timedIndexCounter == 0) {
            if(dependentDeviceId > 0) { //if there's a dependent device
                //find dependent device - call DeviceDelegate
                bool temp = dependentDeviceObject->getDeviceState();
                
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
                    //if there's a dependent device
                    bool temp = dependentDeviceObject->getDeviceState();
                    
                    if(temp) {
                        switchOn();
                    } else {
                        switchOff();
                    }

                } else {
                    // if no dependent
                    deviceState = false;
                    
                    for(uint8_t k=0; k < CHANNEL; k++) {
                        if(color[k].currentColor > 0) {
                            deviceState = true;
                            break;
                        }
                    }
                    
                    if(currentTime == eventTime && oneTime == false) {
                        oneTime = true;
                        initMillis = millis();
                        
                        for(uint8_t k=0; k < CHANNEL; k++) {
                            color[k].colorStartTime = initMillis;
                        }

                        //in case no duration
                        if(eventTime == durationTime) {
                            for(uint8_t k=0; k < CHANNEL; k++) {
                                ledcWrite(color[k].pwmChannel, color[k].pwm[i]);
                            }
                            
                            oneTime = false;
                        }
                        
                        

                    }
                       
                    if(currentTime >= eventTime && currentTime < durationTime && oneTime == true) {
                        long fadeSpan = (durationTime - eventTime) * 1000L;
                        long currentMillis = millis();
                        float percent = (float)(currentMillis-initMillis)/(float)fadeSpan;
                        
                        for(uint8_t k=0; k < CHANNEL; k++) {
                            if(color[k].pin > UNSET) {
                                int colorDif = color[k].pwm[i] - color[k].initColor; // can be + -
                                long colorInterval = fadeSpan/abs(colorDif);
                                
                                if(currentMillis - color[k].colorStartTime >= colorInterval) {
                                    
                                    if(percent >= 0.0f && percent <= 1.0f) {
                                        color[k].currentColor = color[k].initColor + colorDif * percent;
                                    }

                                     ledcWrite(color[k].pwmChannel, color[k].currentColor);

                                    color[k].colorStartTime += colorInterval;
                                }
                                
                            }
                            
                        }

                        

                    } else if(currentTime == durationTime && oneTime == true) {
                        //reset and initialize for next event
                        oneTime = false;
                        
                        for(uint8_t k=0; k < CHANNEL; k++) {
                            if(color[k].pin > UNSET) {
                                color[k].currentColor = color[k].pwm[i];
                                color[k].initColor = color[k].currentColor;

                                ledcWrite(color[k].pwmChannel, color[k].currentColor);
                            }
                            
                        }

                    }
                    
                }// end dependentDevice
            }//end isDay

        }// end for loop
    }// end suspendTime
}


void PWM4::setEvent(char *in_string)
{
    if(in_string[0] == '\0') return;
    
    // start time, duration, pwm
    // "08:00:00,01:00:00,1023:1023:1023:1023"
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
    timedIndexCounter = i/3;

    for (int l=0; l < timedIndexCounter; l++) {
        
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
        
        int tempArr3[CHANNEL] = {0};
        stripTime(events[j], tempArr3);
        for(int k=0; k < CHANNEL; k++) {
            color[k].pwm[l] = tempArr3[k];
        }
        j++;

    }

    /* for Arduino sscanf but the top code will work for anybody
    for (int l=0; l < timedIndexCounter; l++) {
        
        Serial.println(events[j]);
        
        sscanf(events[j], "%d:%d:%d", &hour[l],&minute[l],&second[l]);
        j++;
        
        Serial.println(events[j]);
        sscanf(events[j], "%d:%d:%d", &hourDuration[l],&minuteDuration[l],&secondDuration[l]);
        j++;
        
        sscanf(events[j], "%d:%d:%d:%d",  &color[0].pwm[l], &color[1].pwm[l], &color[2].pwm[l], &color[3].pwm[l]);
        j++;
        
    }
    */
    

    
    

}


void PWM4::getEvent(char *string) {
//Serial.println("get PWM4 event");
    if(timedIndexCounter > 0) {
        
        char buf[41];
        
        strcpy(string, dow);
        
        for (uint8_t i=0; i < timedIndexCounter; i++) {
           sprintf(buf, ",%02d:%02d:%02d,%02d:%02d:%02d,%d:%d:%d:%d", hour[i],minute[i],second[i],hourDuration[i],minuteDuration[i],secondDuration[i],color[0].pwm[i], color[1].pwm[i], color[2].pwm[i], color[3].pwm[i]);
           strcat(string,buf);
        }
        
    } else {
        string[0] = '\0';
        
    }
    
}

int PWM4::getDependentDevice() {
    return dependentDeviceId;
    
}

void PWM4::setDependentDevice(int id) {
    dependentDeviceId = id;
    if(dependentDeviceId > 0) {
        dependentDeviceObject = deviceDelegate.findDevice(dependentDeviceId);
    } else {
        dependentDeviceObject = NULL;
    }

    
}

void PWM4::setSuspendTime(bool in_suspend) {
    suspendTime = in_suspend;
    
    //reset leds
    for(uint8_t k=0; k < CHANNEL; k++) {
        if(color[k].pin > UNSET) {
            ledcWrite(color[k].pwmChannel, color[k].currentColor); 

        }
    }
    //Serial.println("end suspendTime");
}



void PWM4::switchOn()
{
    // address is defined in the device cpp file
    //Serial.println("switching on");
    
    for(uint8_t k=0; k < CHANNEL; k++) {
        if(color[k].pin > UNSET) {
            ledcWrite(color[k].pwmChannel, maxBit);
        }
    }

    
    
    deviceState = true;
}

void PWM4::switchOff()
{
    //Serial.println("switching off");
    
    for(uint8_t k=0; k < CHANNEL; k++) {
        if(color[k].pin > UNSET) {
            ledcWrite(color[k].pwmChannel, 0);
        }
    }


    deviceState = false;
}

void PWM4::toggleState() {
    if (deviceState) {
        switchOff();
        
    } else {
        switchOn();
    }
}

void PWM4::setPWMs(int in_red, int in_green, int in_blue, int in_white)
{
    int tempColor[8] = {in_red, in_green, in_blue, in_white};
    
    for(uint8_t k=0; k < CHANNEL; k++) {
        if(color[k].pin > UNSET) {
            ledcWrite(color[k].pwmChannel, tempColor[k]);
         }
    }

                        
    
}


