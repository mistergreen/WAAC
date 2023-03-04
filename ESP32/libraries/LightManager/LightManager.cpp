/*
 // © Andrea Villa
// villa.andrea@gmail.com
*/

#define UNSET -1

#include "LightManager.h"

const char* LightManager::sNAME_BUTTON_ACTIONS[] = {"Toggle State", "Set to auto"};

LightManager::LightManager(char *in_name, int in_dependent_device_id) : 
    Device(), EventHandler(in_dependent_device_id)
{
    //deviceID is automatically set my deviceDeleGate

    strcpy(deviceName, in_name);
    //classType inherit from base
    strcpy(classType,"LightManager");

    pwmsIndexCounter = 0;
    pwmsDarkIndexCounter = 0;

    color.currentColor = 0;
    color.initColor = 0;
    color.pwmChannel = UNSET;
    color.pin = UNSET;

    colorDark.currentColor = 0;
    colorDark.initColor = 0;
    colorDark.pwmChannel = UNSET;
    colorDark.pin = UNSET;
 
    pin = UNSET;
    relayPin = UNSET;
    sensorPin = UNSET;

    relayInvert = false;
    relayState = false;
    deviceState = false;
    lightSensorValue = false;

    darkMode = false;
    manualMode = false;

    darkEvent.setDependentDevice(in_dependent_device_id);
}


LightManager::~LightManager() {
    //clean up
    switchOff();
}


void LightManager::setLightControlPin(int pin, int channel) {
    //turn off any previous pin
    switchOff();

    color.pin = pin;
    color.pwmChannel = channel;
    
    // Set the pin and attach pin to channel
    if(color.pin > UNSET) {
        pinMode(color.pin, OUTPUT);
        ledcSetup(color.pwmChannel, LEDC_BASE_FREQ, LEDC_TIMER_BIT);
        ledcAttachPin(color.pin, color.pwmChannel);
    }
}


void LightManager::getLightControlPin(int *pin, int *channel) 
{
    *pin = color.pin;
    *channel = color.pwmChannel;
}


void LightManager::setRelayPin(int pin)
{
    relayPin = pin;

    pinMode(pin, OUTPUT);
    digitalWrite(pin,LOW); // weird where pin gets high on startup
                           // devices need pullup or pulldown for desired output
}

int LightManager::getRelayPin()
{
    return relayPin;
}

void LightManager::setSensorPin(int pin)
{
    sensorPin = pin;

    pinMode(pin, INPUT);

    lastDebounceTime = 0;  // the last time the output pin was toggled
    debounceDelay = 50; 
    lastSensorState = LOW; 
}

int LightManager::getSensorPin()
{
    return sensorPin;
}



void LightManager::loop()
{
    bool temp = getNewValue();

    //debounce code - do it with code instead of hardware
    // If the switch changed, due to noise or pressing:
    if(temp != lastSensorState) 
    {
        // reset the debouncing timer
        lastDebounceTime = millis();
    }

    if ((millis() - lastDebounceTime) > debounceDelay) {
        // whatever the reading is at, it has been there for longer than the debounce
        // delay, so take it as the actual current state

         if (temp != lightSensorValue)
         {
            lightSensorValue = temp;

            if (true == lightSensorValue)
            {
                switchToDarkMode();
            }
            else
            {
                switchToNormalMode();
            }
         }
    }

    // save the reading. Next time through the loop, it'll be the lastState:
    lastSensorState = temp;

    EventHandler::loop();

    darkEvent.loop();
}


void LightManager::performActionInEventNoSchedule()
{
    switchOn();
}


void LightManager::performActionOutEventNoSchedule()
{
    switchOff();
}


void LightManager::performActionInEvent()
{
    if(color.pin > UNSET) {
        int colorDif = color.pwm[inProgressEventId] - color.initColor; // can be + -

        // Get the percentage of event reached.
        float percentage = getEventPercentage();

        if(percentage >= 0.0f && percentage <= 1.0f) {
            //Calculate the next color
            int nextColor = color.initColor + colorDif * percentage;

            // If the new color is different then peform the change.
            if (nextColor != color.currentColor)
            {
                color.currentColor = nextColor;
                setPin(color.pwmChannel, color.currentColor);
            }
        }
    }

    deviceState = true;
}


void LightManager::performActionOutEvent()
{
    // Action to perform when turning off
    if ((true == deviceState) && (inProgressEventId > 0) && (inProgressEventId < sMAX_NUM_EVENTS))
    {
        // Store the maximum value for the event.
        if(color.pin > UNSET) {
            color.currentColor = color.pwm[inProgressEventId];
            color.initColor = color.currentColor;

            setPin(color.pwmChannel, color.currentColor);

            if (0 == color.currentColor)
            {
                relaySwitchOff();
            }
            else
            {
                relaySwitchOn();
            }
        }

        deviceState = false;
    }
}


void LightManager::deserializePwms(char *in_string)
{
    if(in_string[0] == '\0')
    {
        Serial.println("LightManager::deserializePwms no events");
        pwmsIndexCounter = 0;
    }
    else
    {
        Serial.print("LightManager::deserializePwms::in_string ");
        Serial.println(in_string);

        //parse incoming string
        char *tok1;
        tok1 = strtok(in_string, ",");

        for (pwmsIndexCounter = 0; (pwmsIndexCounter < sMAX_NUM_EVENTS && tok1 != NULL); pwmsIndexCounter++) 
        {
            sscanf(tok1, "%d", &color.pwm[pwmsIndexCounter]);
            tok1 = strtok(NULL, ",");
        }
    }
}


void LightManager::serializePwms(char *string)
{
    if(pwmsIndexCounter > 0) 
    {    
        char buf[sPWMS_BUFFER_SIZE];

        memset(buf, 0, sizeof(buf));
        memset(string, 0, sizeof(buf));
        
        for (uint8_t i=0; i < pwmsIndexCounter; i++) {
            if (i > 0)
            {
                strcat(string, ",");
            }

            sprintf(buf, "%d", color.pwm[i]);
            strcat(string, buf);
        }
    } 
    else 
    {
        string[0] = '\0';  
    }
}


void LightManager::deserializeDarkPwms(char *in_string)
{
    if(in_string[0] == '\0')
    {
        Serial.println("LightManager::deserializeDarkPwms no events");
        pwmsDarkIndexCounter = 0;
    }
    else
    {
        Serial.print("LightManager::deserializeDarkPwms::in_string ");
        Serial.println(in_string);

        //parse incoming string
        char *tok1;
        tok1 = strtok(in_string, ",");

        for (pwmsDarkIndexCounter = 0; (pwmsDarkIndexCounter < sMAX_NUM_EVENTS && tok1 != NULL); pwmsDarkIndexCounter++) 
        {
            sscanf(tok1, "%d", &colorDark.pwm[pwmsDarkIndexCounter]);
            tok1 = strtok(NULL, ",");
        }
    }
}


void LightManager::serializeDarkPwms(char *string)
{
    if(pwmsDarkIndexCounter > 0) 
    {    
        char buf[sPWMS_BUFFER_SIZE];

        memset(buf, 0, sizeof(buf));
        memset(string, 0, sizeof(buf));
        
        for (uint8_t i=0; i < pwmsDarkIndexCounter; i++) {
            if (i > 0)
            {
                strcat(string, ",");
            }

            sprintf(buf, "%d", colorDark.pwm[i]);
            strcat(string, buf);
        }
    } 
    else 
    {
        string[0] = '\0';  
    }
}


void LightManager::switchOn()
{
    // address is defined in the device cpp file
    Serial.println("switching on");

    relaySwitchOn();
    
    if(color.pin > UNSET) {
        setPin(color.pwmChannel, sMAX_BIT);
    }

    deviceState = true;
}

void LightManager::switchOff()
{
    Serial.println("switching off");

    relaySwitchOff();

    if(color.pin > UNSET) {
        setPin(color.pwmChannel, 0);
    }

    deviceState = false;
}

void LightManager::relaySwitchOn()
{       
    if (false == relayState)
    {       
        Serial.println("LightManager::relaySwitchOn switching on");

        if(relayInvert) {
            digitalWrite(pin,LOW);
        } else {
            digitalWrite(pin,HIGH);
        }

        relayState = true;
    }
}

void LightManager::relaySwitchOff()
{
    if(true == relayState)
    {
        Serial.println("LightManager::relaySwitchOff switching off");

        if(relayInvert) {
            digitalWrite(pin,HIGH);
        } else {
            digitalWrite(pin,LOW);
        }

        relayState = false;
    }
}

void LightManager::toggleState() {
    EventHandler::setSuspendTime(true);

    if (true == deviceState) 
    {
        switchOff();    
    } else 
    {
        switchOn();
    }
}

boolean LightManager::getInvert() {
    return relayInvert;
}

void LightManager::setInvert(boolean state) {
    relayInvert = state;
}

void LightManager::setPWM(int pwm)
{
    if(color.pin > UNSET) {
        setPin(color.pwmChannel, pwm);
    }
}

void LightManager::setSuspendTime(
    bool in_suspend)
{

    manualMode = in_suspend;

    if (false == in_suspend)
    {
        if(true == darkMode)
        {
            EventHandler::setSuspendTime(true);
            darkEvent.setSuspendTime(false);
        }
        else
        {
            EventHandler::setSuspendTime(false);
            darkEvent.setSuspendTime(true);
        }
    }
    else
    {
            EventHandler::setSuspendTime(in_suspend);
    darkEvent.setSuspendTime(in_suspend);
    }
}

bool LightManager::getSuspendTime()
{
    bool suspendTime = false;
    
    if (true == darkMode)
    {
        suspendTime = darkEvent.getSuspendTime();
    }
    else
    {
        suspendTime = EventHandler::getSuspendTime();
    }
     
    return suspendTime;
}


void LightManager::setDependentDevice(int id)
{
    EventHandler::setDependentDevice(id);
    darkEvent.setDependentDevice(id);
}


void LightManager::setPin(uint8_t channel, uint16_t color)
{
    char setPinsLog[512];
    sprintf (setPinsLog, "%s channel %d, color %d", 
            __PRETTY_FUNCTION__ , 
            channel,
            color);
    Serial.println(setPinsLog);

    ledcWrite(channel, color);
}

bool LightManager::getNewValue() 
{
    bool retVal = false;
    
    if (sensorPin >= 0)
    {
        retVal = digitalRead(sensorPin);
    }

    return retVal;
}


void LightManager::serialize(JsonObject& doc)
{
    // First call father serialization
    Device::serialize(doc);
    EventHandler::serialize(doc);

    // 64 characters per event + carriage return
    char darkEventBuff[67*12];

    // clean the buffer
    memset(darkEventBuff, 0, sizeof(darkEventBuff));

    int darkDependentDeviceId = 0;

    bool darkSuspendTime = false;

    darkEvent.serializeBuffer(darkEventBuff, &darkDependentDeviceId, &darkSuspendTime);

    doc["darkEvent"] = darkEventBuff;
    doc["darkDependentDeviceId"] = darkDependentDeviceId;
    doc["darkSuspendTime"] = darkSuspendTime;

    char event[sPWMS_BUFFER_SIZE];
    // clean the buffer
    memset(event, 0, sizeof(event));

    char darkEventString[sPWMS_BUFFER_SIZE];
    // clean the buffer
    memset(darkEventString, 0, sizeof(darkEventString));

    serializePwms(event);
    serializeDarkPwms(darkEventString);
    
    Serial.print("LightManager::serialize event= ");
    Serial.print(event);
    Serial.print(" darkEvent= ");
    Serial.println(darkEventString);

    doc["pin0"] = color.pin;
    doc["cha0"] = color.pwmChannel;

    doc["eventPwms"] = event;
    doc["eventDarkPwms"] = darkEventString;

    doc["relayPin"] = relayPin;
    doc["sensorPin"] = sensorPin;
    doc["relayInvert"] = relayInvert;


}


void LightManager::deserialize(
    JsonObject& doc)
{
   // First call father deserialization
    Device::deserialize(doc);
    EventHandler::deserialize(doc);

    // 64 characters per event + carriage return
    char darkEventBuffer[67*12];
    
    // clean the buffer
    memset(darkEventBuffer, 0, sizeof(darkEvent));

    strcpy (darkEventBuffer, doc["darkEvent"]);

    darkEvent.setEvent(darkEventBuffer);

    darkEvent.setDependentDevice(doc["darkDependentDeviceId"]);

    darkEvent.setSuspendTime(doc["darkSuspendTime"]);

    char event[sPWMS_BUFFER_SIZE];
    
    // clean the buffer
    memset(event, 0, sizeof(event));
    strcpy (event, doc["eventPwms"]);

    char darkEventPwms[sPWMS_BUFFER_SIZE];
    
    // clean the buffer
    memset(darkEventPwms, 0, sizeof(darkEventPwms));
    strcpy (darkEventPwms, doc["eventDarkPwms"]);

    setLightControlPin((int)doc["pin0"], (int)doc["cha0"]);
    
    deserializePwms(event);
    deserializeDarkPwms(darkEventPwms);

    setRelayPin(doc["relayPin"]);
    setSensorPin(doc["sensorPin"]);
    setInvert(doc["relayInvert"]);
}


int LightManager::getNumButtonActions()
{
    return sNUM_BUTTON_ACTIONS;
}


const char* LightManager::getButtonActionName(
    int actionId)
{
    // The returned value.
    char* retVal = "";

    // Check the id before parforming any action.
    if (actionId < sNUM_BUTTON_ACTIONS)
    {
        retVal = (char *) sNAME_BUTTON_ACTIONS[actionId];
    }

    return retVal;
}


void LightManager::callButtonAction(
    int actionId)
{   
    Serial.print("Device::callButtonAction calling action ");
    Serial.println(actionId);
    
    // Check the id before parforming any action.
    if (actionId == 0)
    {
        toggleState();
    }
    else if (actionId == 1)
    {
        EventHandler::setSuspendTime(false);
    }
}


void LightManager::setEventDark(char *in_string)
{
    darkEvent.setEvent(in_string);
}

void LightManager::getEventDark(char *in_string)
{
    darkEvent.getEvent(in_string);
}

void LightManager::switchToDarkMode()
{
    darkMode = true;

    Serial.println("LightManager::switchToNormalMode switching to dark");

    EventHandler:setSuspendTime(true);
    darkEvent.setSuspendTime(false);
}

void LightManager::switchToNormalMode()
{
    darkMode = false;

    Serial.println("LightManager::switchToNormalMode switching to normal");

    EventHandler:setSuspendTime(false);
    darkEvent.setSuspendTime(true);
}

bool LightManager::getDarkModelMode()
{
    return darkMode;
}