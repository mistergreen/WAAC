/*
* The scheduling class cpp file.
*/

//#include <TimeLib.h>
#include "EventHandler.h"
#include "WWWsettings.h"
#include <ezTime.h>

// The event string size is 9(hh:mm:ss,) + 9(hh:mm:ss,) + 8(dow) + 1 (string terminator) = 27
#define EVENT_STRING_SIZE 27


EventHandler::EventHandler()
{
    timedIndexCounter = 0;
    dependentDeviceObject = NULL;
    suspendTime = false;
    inProgressEventId = -1;
    lastEventId = -1;
}


EventHandler::EventHandler(int in_dependent_device_id)
{
    timedIndexCounter = 0;

    // Device() call super to init vars
    dependentDeviceId = in_dependent_device_id;
    //find dependent device once - not all the time
    setDependentDevice(dependentDeviceId);
    //deviceID is automatically set my deviceDeleGate

    suspendTime = false;
    inProgressEventId = -1;
    lastEventId = -1;
}


bool EventHandler::areScheduledEvents()
{
    // The returned value, true if there are events.
    bool retVal = false;
    
    // Check if there are events scheduled.
    if (timedIndexCounter > 0)
    {
        retVal = true;
    }
    
    return retVal;
}


float EventHandler::getEventPercentage()
{
    // The returned percentage.
    float percentage = 0;

    // Check the event in progress value for array boundaries.
    if ((0 <= inProgressEventId) && (sMAX_NUM_EVENTS > inProgressEventId))
    {
        // Get start time in milliseconds.
        long eventTime = convertToSeconds(hour[inProgressEventId],
                                        minute[inProgressEventId],
                                        second[inProgressEventId]);

        // Calculate the event duration in milliseconds.
        long evenDuration =  convertToSeconds(hourDuration[inProgressEventId],
                                            minuteDuration[inProgressEventId],
                                            secondDuration[inProgressEventId]);

        // Get the settings instance.
        WWWsettings* settings = WWWsettings::getinstance();

        // Get the current time.
        Timezone* now = settings->getTime();

        // Get the current time.        
        long currentTime = convertToSeconds(now->hour(), now->minute(), now->second());

        percentage = (float)(currentTime-eventTime) / (float)evenDuration;

        // Clip for possible rounding problems.
        if (percentage < 0)
        {
            percentage = 0;
        }
        else if (percentage > 1)
        {
            percentage = 1;
        }

    }
    
    return percentage;
}


void EventHandler::performActionInEventNoSchedule()
{
    performActionInEvent();
}


void EventHandler::performActionOutEventNoSchedule()
{
    performActionOutEvent();
}


bool EventHandler::isEventInProgress()
{
    // The returned value, it is true if there is a scheduled event now.
    bool retVal = false;

    if ((-1 < inProgressEventId) && (sMAX_NUM_EVENTS > inProgressEventId))
    {
        char currentEventDow = (dow[inProgressEventId][::weekday()-1]);
              
        //Serial.println("isDay of event");
        // ::hour() get time from Time.h
        
        // Get the settings instance.
        WWWsettings* settings = WWWsettings::getinstance();

        // Get the current time.
        Timezone* now = settings->getTime();

        long currentTime = convertToSeconds(now->hour(), now->minute(), now->second());

        // Get start time
        long eventTime = convertToSeconds(hour[inProgressEventId],
                                        minute[inProgressEventId],
                                        second[inProgressEventId]);
        // Calculate end time.
        long durationTime = convertToSeconds(hourDuration[inProgressEventId],
                                            minuteDuration[inProgressEventId],
                                            secondDuration[inProgressEventId]) + eventTime;
            
        // Rollover midnight
        if (durationTime > 86400L) {
            durationTime = durationTime - 86400L;
        }
            
        // Check if there is an event in progress now.
        if ((currentEventDow == '1') && (currentTime >= eventTime) && (currentTime <= durationTime)) {
            retVal = true;
        }
        else {
            // Store the last event id and reset the current one.
            lastEventId = inProgressEventId;
            inProgressEventId = -1;
        }
    }
    
    // Loop through events and check if there is one triggered
    for (uint8_t i = 0; (i < timedIndexCounter) && (false == retVal); i++) {
        
        // dow comming in '1111110' 7 places 0 - sunday
        // isDay is the day an event is to occur
        // Time.h sunday is 1
        if(dow[i][::weekday()-1] == '1') {
            //Serial.println("isDay of event");
            // ::hour() get time from Time.h
            // Get the settings instance.
            WWWsettings* settings = WWWsettings::getinstance();

            // Get the current time.
            Timezone* now = settings->getTime();

            long currentTime = convertToSeconds(now->hour(), now->minute(), now->second());
            // Get start time
            long eventTime = convertToSeconds(hour[i],minute[i],second[i]);
            // Calculate end time.
            long durationTime = convertToSeconds(hourDuration[i],minuteDuration[i],secondDuration[i]) + eventTime;
            
            // Rollover midnight
            if (durationTime > 86400L) {
                durationTime = durationTime - 86400L;
            }
            
            // Check if there is an event in progress now.
            if(currentTime >= eventTime && currentTime <= durationTime) {
                retVal = true;
                inProgressEventId = i;
            }
        } else {
            //Serial.println("notDay!");
        }
    }

    return retVal;
}


void EventHandler::loop()
{
    // If timer is not suspended then check scheduler and dependent devices.
    if(suspendTime == false) {
        // Check if there is an event in progress now.
        if (false == areScheduledEvents()) {
            
            // if there's a dependent device
            if(NULL != dependentDeviceObject) {
                //find dependent device - call DeviceDelegate
                if(true == dependentDeviceObject->getDeviceState()) {
                    performActionInEventNoSchedule();
                } else {
                    performActionOutEventNoSchedule();
                }
            }
        }
        else if (true == isEventInProgress())
        {
            // if there's a dependent device
            if(NULL != dependentDeviceObject) { 
                // find dependent device - call DeviceDelegate
                if(true == dependentDeviceObject->getDeviceState()) {
                    performActionInEvent();
                } else {
                    performActionOutEvent();
                }
            }
            else {
                performActionInEvent();
            }
        }
        else {
            performActionOutEvent();
        }
    }// end suspend time
}


void EventHandler::setEvent(char *in_string)
{

    if(in_string[0] == '\0'){
        Serial.println("EventHandler::setEvent no events");
        timedIndexCounter = 0;
    }
    else
    {
        Serial.println("EventHandler::setEvent::in_string");
        Serial.println(in_string);

        // start time, duration, dow
        // "8:00:00,1:00:00,1111111,9:30:00,1:30:01,1101111,15:34:02,1:05:02,1010101"
        //you can only add up to 4 events- each event is -on and duration -off pairs
        Serial.println("out event");
        char events[12][67];
        
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
        
        timedIndexCounter = uint8_t(i/3);
        Serial.println("timedIndexCounter");
        Serial.println(timedIndexCounter);

        if (sMAX_NUM_EVENTS > timedIndexCounter)
        {
            for (uint8_t l=0; l < timedIndexCounter; l++) {
                sscanf(events[j], "%d:%d:%d", &hour[l],&minute[l],&second[l]);
                j++;
                sscanf(events[j], "%d:%d:%d", &hourDuration[l],&minuteDuration[l],&secondDuration[l]);
                j++;
                strcpy(dow[l],events[j]);
                j++;
                
            }
        }
    }
}


void EventHandler::getEvent(char *string) {
    //manipulat incoming string rather than returning. Saves memory?
    if(timedIndexCounter > 0) {
        Serial.println("EventHandler::getEvent timedIndexCounter");
        Serial.println(timedIndexCounter);
        
        // Define a temporary string buffer.
        char buf[EVENT_STRING_SIZE];

        // clean the buffer
        memset(buf, 0, sizeof(buf));
        memset(string, 0, sizeof(buf));
        
        for (uint8_t i=0; i < timedIndexCounter; i++) {
            if (i > 0)
            {
                strcat(string, ",");
            }
            sprintf(buf, "%02d:%02d:%02d,%02d:%02d:%02d,%s", hour[i],minute[i],second[i],hourDuration[i],minuteDuration[i],secondDuration[i],dow[i]);
            strcat(string,buf);
        }
    } else {
        Serial.println("EventHandler::getEvent no event");
        strcpy(string, "");
    }
}


int EventHandler::getDependentDevice() {
    return dependentDeviceId;
    
}

void EventHandler::setDependentDevice(int id) {
    dependentDeviceId = id;
    if(dependentDeviceId > 0) {
        dependentDeviceObject = deviceDelegate.findDevice(dependentDeviceId);
    } else {
        dependentDeviceObject = NULL;
    }
}


void EventHandler::setSuspendTime(bool in_suspend)
{
    suspendTime = in_suspend;
}


void EventHandler::serialize(JsonObject& doc)
{
    // 64 characters per event + carriage return
    char event[67*12];

    // clean the buffer
    memset(event, 0, sizeof(event));

    getEvent(event);
    
    Serial.print("EventHandler::serialize ");
    Serial.println(event);

    doc["event"] = event;
    doc["dependentDeviceId"] = dependentDeviceId;
}

void EventHandler::deserialize(
    JsonObject& doc)
{
    // 64 characters per event + carriage return
    char event[67*12];
    
    // clean the buffer
    memset(event, 0, sizeof(event));

    strcpy (event, doc["event"]);

    setEvent(event);

    setDependentDevice(doc["dependentDeviceId"]);
}


long EventHandler::convertToSeconds(int in_hour, int in_minute, int in_second) {
    return in_hour*3600L +in_minute*60L +in_second;
}


void EventHandler::stripTime(char *inString, int *inArray) {
    char *tok1;
    int i = 0;
    tok1 = strtok(inString, ":");
    while (tok1 != NULL) {
        inArray[i] = atoi(tok1);
        tok1 = strtok(NULL, ":");
        i++;
        
    }
}