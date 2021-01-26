/*
 // © 2noodles llc
 // minh@2noodles.com
*/

#define UNSET -1

#include "PWM4.h"


PWM4::PWM4(char *in_name, int in_dependent_device_id, uint8_t channels, char *format) : 
    Device(), EventHandler(in_dependent_device_id), NUM_CHANNELS (channels)
{
    //deviceID is automatically set my deviceDeleGate

    strcpy(deviceName,in_name);
    //classType inherit from base
    strcpy(classType,"PWM4");

    channelsFormat = new char[strlen(format)+1];
    strcpy(channelsFormat, format);

    color = new colorAux[NUM_CHANNELS];

    timedIndexCounter = 0;
    
    for(int i=0; i < NUM_CHANNELS;i++) {
        color[i].currentColor = 0;
        color[i].initColor = 0;
    }
    
    pin = 0;
}

PWM4::~PWM4() {
    //clean up
    switchOff();

    delete channelsFormat;

    delete color;
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
    for(uint8_t i = 0; i < NUM_CHANNELS; i++) {
        if(color[i].pin > UNSET) pinMode(color[i].pin, OUTPUT);
        
    }

    
    //attachpins to channels - pin/channel pair
    
    for(uint8_t i = 0; i < NUM_CHANNELS; i++) {
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
    for(int i=0; i < NUM_CHANNELS; i++) {
        inPinArray[i] = color[i].pin;
        inChannelArray[i] = color[i].pwmChannel;
    }
}


void PWM4::loop()
{
    EventHandler::loop();
}


void PWM4::performActionInEventNoSchedule()
{
    switchOn();
}


void PWM4::performActionOutEventNoSchedule()
{
    switchOff();
}


void PWM4::performActionInEvent()
{
    for(uint8_t k=0; k < NUM_CHANNELS; k++) {
        if(color[k].pin > UNSET) {
            int colorDif = color[k].pwm[inProgressEventId] - color[k].initColor; // can be + -

            // Get the percentage of event reached.
            float percentage = getEventPercentage();

            if(percentage >= 0.0f && percentage <= 1.0f) {
                //Calculate the next color
                int nextColor = color[k].initColor + colorDif * percentage;

                // If the new color is different then peform the change.
                if (nextColor != color[k].currentColor)
                {
                    color[k].currentColor = nextColor;

                    setPin(color[k].pwmChannel, color[k].currentColor);
                }
            }
        }
    }

    deviceState = true;
}


void PWM4::performActionOutEvent()
{
    // Action to perform when turning off
    if ((true == deviceState) && (inProgressEventId > 0) && (inProgressEventId < sMAX_NUM_EVENTS))
    {
        // For each channel store the maximum value for the event.
        for(uint8_t k=0; k < NUM_CHANNELS; k++) {
            if(color[k].pin > UNSET) {
                color[k].currentColor = color[k].pwm[inProgressEventId];
                color[k].initColor = color[k].currentColor;

                setPin(color[k].pwmChannel, color[k].currentColor);
            }
        }

        deviceState = false;
    }
}


void PWM4::setEventColors(char *in_string)
{
    if(in_string[0] == '\0') return;

        // start time, duration, pwm
    // "08:00:00,01:00:00,1023:1023:1023:1023"
    //you can only add up to 4 events- each event is -on and duration -off pairs

    char events[4][20];
    
    //Serial.println(in_string);

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
    timedIndexCounter = i/3;

    //for Arduino sscanf but the top code will work for anybody
    for (int l=0; l < timedIndexCounter; l++) {
        Serial.println(events[j]);
        
        sscanf(events[j], channelsFormat,  &color[0].pwm[l], &color[1].pwm[l], &color[2].pwm[l], &color[3].pwm[l]);
        j++;
    }
}


void PWM4::getEventColors(char *string)
{
    if(timedIndexCounter > 0) {
        
        char buf[16];
        
        for (uint8_t i=0; i < timedIndexCounter; i++) {
            if (i >0 )
            {
                strcat(string, ",");
            }

            sprintf(buf, channelsFormat,color[0].pwm[i], color[1].pwm[i], color[2].pwm[i], color[3].pwm[i]);
            strcat(string, buf);
        }
        
    } else {
        string[0] = '\0';
        
    }
}


void PWM4::switchOn()
{
    // address is defined in the device cpp file
    //Serial.println("switching on");
    
    for(uint8_t k=0; k < NUM_CHANNELS; k++) {
        if(color[k].pin > UNSET) {
            setPin(color[k].pwmChannel, maxBit);
        }
    }

    deviceState = true;
}

void PWM4::switchOff()
{
    //Serial.println("switching off");
    
    for(uint8_t k=0; k < NUM_CHANNELS; k++) {
        if(color[k].pin > UNSET) {
            setPin(color[k].pwmChannel, 0);
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
    
    for(uint8_t k=0; k < NUM_CHANNELS; k++) {
        if(color[k].pin > UNSET) {
            setPin(color[k].pwmChannel, tempColor[k]);
         }
    }
}

void PWM4::setPin(uint8_t channel, uint16_t color)
{
    ledcWrite(channel, color);
}


void PWM4::serialize(JsonObject& doc)
{
    // First call father serialization
    Device::serialize(doc);
    
    doc["pin0"] = color[0].pin;
    doc["pin1"] = color[1].pin;
    doc["pin2"] = color[2].pin;
    doc["pin3"] = color[3].pin;

    doc["cha0"] = color[0].pwmChannel;
    doc["cha1"] = color[1].pwmChannel;
    doc["cha2"] = color[2].pwmChannel;
    doc["cha3"] = color[3].pwmChannel;

    char event[4*20];
    getEventColors(event);
    doc["eventColors"] = event;
}

void PWM4::deserialize(
    JsonObject& doc)
{
   // First call father deserialization
    Device::deserialize(doc);

    setPins(doc["pin0"],
            doc["pin1"],
            doc["pin2"],
            doc["pin3"],
            doc["cha0"],
            doc["cha1"],
            doc["cha2"],
            doc["cha3"]);

    char event[4*20];
    strcpy (event, doc["eventColors"]);
    
    setEventColors(event);
    }