/*
 // © Andrea Villa
// villa.andrea@gmail.com
*/

#define UNSET -1

#include "LightManager.h"

const char* LightManager::sNAME_BUTTON_ACTIONS[] = {"Toggle State", "Set to auto"};

LightManager::LightManager(char *in_name, int in_dependent_device_id) : 
    Device()
{
    //deviceID is automatically set my deviceDeleGate

    strcpy(deviceName, in_name);
    //classType inherit from base
    strcpy(classType,"LightManager");
 
    pin = UNSET;
    relayPin = UNSET;
    sensorPin = UNSET;
    pwmPin = UNSET;
    pwmChannel = UNSET;

    relayInvert = false;
    relayState = false;
    deviceState = false;
    lightSensorValue = false;

    darkMode = false;
    manualMode = false;

    normalEvent = new LMEventManager(this, in_dependent_device_id);
    darkEvent = new LMEventManager(this, in_dependent_device_id);

    currentLM = normalEvent;
}


LightManager::~LightManager() {
    //clean up
    switchOff();
}


void LightManager::setLightControlPin(int pin, int channel) {
    //turn off any previous pin
    switchOff();

    pwmPin = pin;
    pwmChannel = channel;
    
    // Set the pin and attach pin to channel
    if((pwmPin > UNSET) && (pwmChannel> UNSET)) {
        pinMode(pwmPin, OUTPUT);
        ledcSetup(pwmChannel, LEDC_BASE_FREQ, LEDC_TIMER_BIT);
        ledcAttachPin(pwmPin, pwmChannel);
    }
}


void LightManager::getLightControlPin(int *pin, int *channel) 
{
    *pin = pwmPin;
    *channel = pwmChannel;
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

    currentLM->loop();
}


void LightManager::deserializePwms(char *in_string)
{
    normalEvent->deserializePwms(in_string);
}


void LightManager::serializePwms(char *string)
{
    normalEvent->serializePwms(string);
}


void LightManager::deserializeDarkPwms(char *in_string)
{
    darkEvent->deserializePwms(in_string);
}


void LightManager::serializeDarkPwms(char *string)
{
    darkEvent->serializePwms(string);
}


void LightManager::switchOn()
{
    // address is defined in the device cpp file
    Serial.println("switching on");

    setPin(sMAX_BIT);
}

void LightManager::switchOff()
{
    Serial.println("switching off");

    setPin(0);
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
    setSuspendTime(true);

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
    setPin(pwm);
}


void LightManager::setSuspendTime(
    bool in_suspend)
{
    manualMode = in_suspend;

    currentLM->setSuspendTime(in_suspend);
}

bool LightManager::getSuspendTime()
{
    bool suspendTime = false;
    
    suspendTime = currentLM->getSuspendTime();
     
    return suspendTime;
}


int LightManager::getDependentDevice()
{
    return currentLM->getDependentDevice();
}

void LightManager::setDependentDevice(int id)
{
    normalEvent->setDependentDevice(id);
    darkEvent->setDependentDevice(id);
}


void LightManager::setPin(uint16_t color)
{
    char setPinsLog[512];
    sprintf (setPinsLog, "%s channel %d, color %d", 
            __PRETTY_FUNCTION__ , 
            pwmChannel,
            color);
    Serial.println(setPinsLog);

    if((pwmPin > UNSET) && (pwmChannel > UNSET)) {
        if (0 == color)
        {
            relaySwitchOff();

            deviceState = false;
        }
        else
        {
            relaySwitchOn();

            deviceState = true;
        }

        ledcWrite(pwmChannel, color);
    }
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
    normalEvent->serialize(doc);

    // 64 characters per event + carriage return
    char darkEventBuff[67*12];

    // clean the buffer
    memset(darkEventBuff, 0, sizeof(darkEventBuff));

    int darkDependentDeviceId = 0;

    bool darkSuspendTime = false;

    darkEvent->serializeBuffer(darkEventBuff, &darkDependentDeviceId, &darkSuspendTime);

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

    doc["pin0"] = pwmPin;
    doc["cha0"] = pwmChannel;

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
    normalEvent->deserialize(doc);

    // 64 characters per event + carriage return
    char darkEventBuffer[67*12];
    
    // clean the buffer
    memset(darkEventBuffer, 0, sizeof(darkEvent));

    strcpy (darkEventBuffer, doc["darkEvent"]);

    darkEvent->setEvent(darkEventBuffer);

    darkEvent->setDependentDevice(doc["darkDependentDeviceId"]);

    darkEvent->setSuspendTime(doc["darkSuspendTime"]);

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
        setSuspendTime(false);
    }
}


void LightManager::setEvent(char *in_string)
{
    normalEvent->setEvent(in_string);
}


void LightManager::getEvent(char *in_string)
{
    normalEvent->getEvent(in_string);
}


void LightManager::setEventDark(char *in_string)
{
    darkEvent->setEvent(in_string);
}


void LightManager::getEventDark(char *in_string)
{
    darkEvent->getEvent(in_string);
}


void LightManager::switchToDarkMode()
{
    darkMode = true;

    Serial.println("LightManager::switchToNormalMode switching to dark");

    darkEvent->setSuspendTime(currentLM->getSuspendTime());

    darkEvent->setCurrentColor(currentLM->getCurrentColor());

    currentLM = darkEvent;

    normalEvent->setSuspendTime(true);
}


void LightManager::switchToNormalMode()
{
    darkMode = false;

    Serial.println("LightManager::switchToNormalMode switching to normal");

    normalEvent->setSuspendTime(currentLM->getSuspendTime());

    normalEvent->setCurrentColor(currentLM->getCurrentColor());

    currentLM = normalEvent;

    darkEvent->setSuspendTime(true);
}


bool LightManager::getDarkModelMode()
{
    return darkMode;
}


LightManager::LMEventManager::LMEventManager(LightManager* lightManager, int in_dependent_device_id) : EventHandler(in_dependent_device_id)
{
    lm = lightManager;

    color.currentColor = 0;
    color.initColor = 0;

    pwmsIndexCounter = 0;
}

LightManager::LMEventManager::~LMEventManager()
{
    lm = static_cast<LightManager*>(0);
}


void LightManager::LMEventManager::setCurrentColor(int currentColor)
{
    lm->setPin(color.currentColor);
    
    color.initColor = currentColor;
    color.currentColor = currentColor;
}


int LightManager::LMEventManager::getCurrentColor()
{
    return color.currentColor;
}


// It is the action performed during an event.
void LightManager::LMEventManager::performActionInEvent()
{
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

            lm->setPin(color.currentColor);
        }
    }
}

// It is the action performed out of an event.
void LightManager::LMEventManager::performActionOutEvent()
{
    // Action to perform when turning off
    if ((inProgressEventId > 0) && (inProgressEventId < sMAX_NUM_EVENTS))
    {
        color.currentColor = color.pwm[inProgressEventId];
        color.initColor = color.currentColor;

        lm->setPin(color.currentColor);
    }
}

void LightManager::LMEventManager::performActionInEventNoSchedule()
{
    lm->switchOn();
}


void LightManager::LMEventManager::performActionOutEventNoSchedule()
{
    lm->switchOff();
}

void LightManager::LMEventManager::deserializePwms(char *in_string)
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


void LightManager::LMEventManager::serializePwms(char *string)
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