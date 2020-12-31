/*
* The scheduling class cpp file.
*/

#include <TimeLib.h>
#include "Scheduler.h"

Scheduler::Scheduler()
{
    timedIndexCounter = 0;
}


bool Scheduler::areScheduledEvents()
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


bool Scheduler::isEventInProgress()
{
    // The returned value, it is true if there is a scheduled event now.
    bool retVal = false;
    
    // Loop through events and check if there is one triggered
    for (uint8_t i = 0; i < timedIndexCounter; i++) {
        
        // dow comming in '1111110' 7 places 0 - sunday
        // isDay is the day an event is to occur
        // Time.h sunday is 1
        if(dow[i][::weekday()-1] == '1') {
            //Serial.println("isDay of event");
            // ::hour() get time from Time.h
            long currentTime = convertToSeconds(::hour(),::minute(),::second());
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
            }
        } else {
            //Serial.println("notDay!");
        }
    }
}



void Scheduler::setEvent(char *in_string)
{
    Serial.println("in event");
    //Serial.println(in_string);
    if(in_string[0] == '\0') return;
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
    
#ifdef ESP8266
    for (uint8_t l=0; l < timedIndexCounter; l++) {
        
        //start
        
        int tempArr1[3] = {0};
        stripTime(events[j], tempArr1);
        hour[l] = tempArr1[0];
        minute[l] = tempArr1[1];
        second[l] = tempArr1[2];
        j++;
        
        
        //duration

        int tempArr2[3] = {0};
        stripTime(events[j], tempArr2);
        hourDuration[l] = tempArr2[0];
        minuteDuration[l] = tempArr2[1];
        secondDuration[l] = tempArr2[2];
        j++;
        
        
        //dow
        strcpy(dow[l],events[j]);
        j++;
        
        yield();
        
    }
#elif ESP32
    for (uint8_t l=0; l < timedIndexCounter; l++) {

        //start
        
        int tempArr1[3] = {0};
        stripTime(events[j], tempArr1);
        hour[l] = tempArr1[0];
        minute[l] = tempArr1[1];
        second[l] = tempArr1[2];
        j++;
        
        
        //duration
        
        int tempArr2[3] = {0};
        stripTime(events[j], tempArr2);
        hourDuration[l] = tempArr2[0];
        minuteDuration[l] = tempArr2[1];
        secondDuration[l] = tempArr2[2];
        j++;
        
        
        //dow
        strcpy(dow[l],events[j]);
        j++;
        
        yield();
        
    }
#else
    
    for (uint8_t l=0; l < timedIndexCounter; l++) {
        sscanf(events[j], "%d:%d:%d", &hour[l],&minute[l],&second[l]);
        j++;
        sscanf(events[j], "%d:%d:%d", &hourDuration[l],&minuteDuration[l],&secondDuration[l]);
        j++;
        strcpy(dow[l],events[j]);
        j++;
        
    }

#endif
    

}


void Scheduler::getEvent(char *string) {
    //manipulat incoming string rather than returning. Saves memory?
    if(timedIndexCounter > 0) {
        Serial.println("crash with event");
        Serial.println("timedIndexCounter");
        Serial.println(timedIndexCounter);
        
        
        
        char buf[104];
        sprintf(buf, "%02d:%02d:%02d,%02d:%02d:%02d,%s", hour[0],minute[0],second[0],hourDuration[0],minuteDuration[0],secondDuration[0],dow[0]);
        strcpy(string, buf);
        
        for (uint8_t i=1; i < timedIndexCounter; i++) {
            sprintf(buf, ",%02d:%02d:%02d,%02d:%02d:%02d,%s", hour[i],minute[i],second[i],hourDuration[i],minuteDuration[i],secondDuration[i],dow[i]);
            strcat(string,buf);
        }
    } else {
        Serial.println("crash no event");
        // strcpy(string, '\0');
    }
}


void Scheduler::serialize(JsonObject& doc)
{
    // 64 characters per event + carriage return
    char event[67*12];
    getEvent(event);
    doc["event"] = event;
}

void Scheduler::deserialize(
    JsonObject& doc)
{
    // 64 characters per event + carriage return
    char event[67*12];
    strcpy (event, doc["event"]);
    
    setEvent(event);
}


long Scheduler::convertToSeconds(int in_hour, int in_minute, int in_second) {
    return in_hour*3600L +in_minute*60L +in_second;
}


void Scheduler::stripTime(char *inString, int *inArray) {
    char *tok1;
    int i = 0;
    tok1 = strtok(inString, ":");
    while (tok1 != NULL) {
        inArray[i] = atoi(tok1);
        tok1 = strtok(NULL, ":");
        i++;
        
    }
}