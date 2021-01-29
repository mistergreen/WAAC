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

    colorsIndexCounter = 0;
    
    for(int i=0; i < NUM_CHANNELS;i++) {
        color[i].currentColor = 0;
        color[i].initColor = 0;
        color[i].pwmChannel = UNSET;
        color[i].pin = UNSET;
    }
    
    pin = 0;
}

PWM4::~PWM4() {
    //clean up
    switchOff();

    delete [] channelsFormat;
    channelsFormat = NULL;

    delete [] color;
    color = NULL;
}

void PWM4::setPins(int red, int green, int blue, int white, int channel0, int channel1, int channel2, int channel3 ) {
    
    //char setPinsLog[512];
    //sprintf (setPinsLog, "%s red %d, green %d, blue %d, white %d, c0 %d, c1 %d, c2 %d, c3 %d", 
    //        __PRETTY_FUNCTION__ , 
    //        red, 
    //        green, 
    //        blue, 
    //        white, 
    //        channel0, 
    //        channel1, 
    //       channel2, 
    //        channel3);
    //Serial.println(setPinsLog);

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

            //Serial.print("PWM4::performActionInEvent colorDif ");
            //Serial.print(colorDif);

            //Serial.print("percentage ");
            //Serial.println(percentage);

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
     if(in_string[0] == '\0'){
        Serial.println("PWM4::setEventColors no events");
        colorsIndexCounter = 0;
    }
    else
    {
        Serial.print("PWM4::setEventColors::in_string ");
        Serial.println(in_string);

        // start time, duration, pwm
        // "08:00:00,01:00:00,1023:1023:1023:1023"
        //you can only add up to 4 events- each event is -on and duration -off pairs

        char events[4][21];
        
        //Serial.println(in_string);

        //parse incoming string *** MAKE ROOM FOR THE NUL TERMINATOR in the string!
        char *tok1;
        int i = 0;
        tok1 = strtok(in_string, ",");
        while (tok1 != NULL) {
            strcpy(events[i], tok1);
            tok1 = strtok(NULL, ",");
            i++;
        }
        
        int j = 0;
        colorsIndexCounter = i;

        Serial.print("colorsIndexCounter ");
        Serial.println(colorsIndexCounter);

        //for Arduino sscanf but the top code will work for anybody
        for (int l=0; l < colorsIndexCounter; l++) {
            Serial.println(events[j]);

            tok1 = strtok(events[j], ":");
            for(uint8_t k=0; k < NUM_CHANNELS; k++)
            {
                sscanf(tok1, "%d", &color[k].pwm[l]);
                tok1 = strtok(NULL, ":");
            }

            j++;
        }
    }
}


void PWM4::getEventColors(char *string)
{
    if(colorsIndexCounter > 0) {
        
        char buf[21];

        memset(buf, 0, sizeof(buf));
        memset(string, 0, sizeof(buf));
        
        for (uint8_t i=0; i < colorsIndexCounter; i++) {
            if (i > 0)
            {
                strcat(string, ",");
            }

            for(uint8_t k=0; k < NUM_CHANNELS; k++)
            {
                sprintf(buf, "%d", color[k].pwm[i]);
                strcat(string, buf);

                if (k < (NUM_CHANNELS - 1))
                {
                    strcat(string, ":");
                }
            }
        }
    } 
    else {
        string[0] = '\0';
        
    }
}


void PWM4::switchOn()
{
    // address is defined in the device cpp file
    Serial.println("switching on");
    
    for(uint8_t k=0; k < NUM_CHANNELS; k++) {
        if(color[k].pin > UNSET) {
            setPin(color[k].pwmChannel, maxBit);
        }
    }

    deviceState = true;
}

void PWM4::switchOff()
{
    Serial.println("switching off");

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
    //char setPinsLog[512];
    //sprintf (setPinsLog, "%s channel %d, color %d", 
    //        __PRETTY_FUNCTION__ , 
    //        channel,
    //        color);
    //Serial.println(setPinsLog);

    ledcWrite(channel, color);
}


void PWM4::serialize(JsonObject& doc)
{
    // First call father serialization
    Device::serialize(doc);
    EventHandler::serialize(doc);

    char event[4*21];
    // clean the buffer
    memset(event, 0, sizeof(event));

    getEventColors(event);

    Serial.print("PWM4::serialize ");
    Serial.println(event);

    doc["eventColors"] = event;
    
    doc["pin0"] = color[0].pin;
    doc["pin1"] = color[1].pin;
    doc["pin2"] = color[2].pin;
    doc["pin3"] = color[3].pin;

    doc["cha0"] = color[0].pwmChannel;
    doc["cha1"] = color[1].pwmChannel;
    doc["cha2"] = color[2].pwmChannel;
    doc["cha3"] = color[3].pwmChannel;
}

void PWM4::deserialize(
    JsonObject& doc)
{
   // First call father deserialization
    Device::deserialize(doc);
    EventHandler::deserialize(doc);

    char event[4*20];
    // clean the buffer
    memset(event, 0, sizeof(event));
    strcpy (event, doc["eventColors"]);
    
    setEventColors(event);

    setPins((int)doc["pin0"],
            (int)doc["pin1"],
            (int)doc["pin2"],
            (int)doc["pin3"],
            (int)doc["cha0"],
            (int)doc["cha1"],
            (int)doc["cha2"],
            (int)doc["cha3"]);
}