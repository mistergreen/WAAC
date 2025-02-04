/*
  Alert
*/

// © 2noodles llc
// minh@2noodles.com


#include "Alert.h"
#include <Device.h>
#include <DeviceDelegate.h>
#include <WWWsettings.h>

 
Alert::Alert() : Device(), Storable()
{
    hour = 1;
    minute = 0;
    
    strcpy(message, "");
    strcpy(subject, "");
    
    //classType inherit from base
    strcpy(classType,"Alert");
    
    intervalTime = 0;
    interval = 0;

}


Alert::Alert(char *in_name, int in_dependent_device_id) : Device(), Storable()
{
    // Device() call super to init vars
    dependentDeviceId = in_dependent_device_id;
    //find dependent device once - not all the time
    if(dependentDeviceId) {
        dependentDeviceObject = deviceDelegate.findDevice(dependentDeviceId);
    }

    hour = 1;
    minute = 0;
    
    strcpy(message, "");
    strcpy(subject, "");
    
    //deviceID is automatically set my deviceDeleGate

    strcpy(deviceName,in_name);
    //classType inherit from base
    strcpy(classType,"Alert");
    
    intervalTime = 0;
    interval = 0;
    

}

Alert::~Alert() {
    //delete client;
    //client = NULL;
}; // destructor


void Alert::loop()
{
    
    if(suspendTime == false) {
        
            if(dependentDeviceId > 0) { //if there's a dependent device
                //find dependent device - call DeviceDelegate
                boolean temp = dependentDeviceObject->getDeviceState();

                // Get the settings instance.
                WWWsettings* settings = WWWsettings::getinstance();
                
                // Get the current time.
                Timezone* now = settings->getTime();

                long currentTime = convertToSeconds(now->hour(),now->minute(),now->second());
                
                if(temp == deviceState && intervalTime < currentTime ) {
                    
                    settings->email(getSubject(), getMessage());
                    intervalTime = interval + currentTime;
                    //if pin is set otherwise UNSET -1
                    if(pin > 0)
                        digitalWrite(pin, HIGH);
                } else if (temp != deviceState && intervalTime < currentTime) {
                    //don't reset right away
                    intervalTime = 0;
                    if(pin > 0)
                        digitalWrite(pin, LOW);
                }
            }
            
        
    }
}
/* uses system's emailer instead*/


void Alert::setEvent(char *in_string) {
    
    if(in_string[0] == '\0') return;
#ifdef ESP8266
    
    int tempArr1[2] = {0};
    stripTime(in_string, tempArr1);
    hour = tempArr1[0];
    minute = tempArr1[1];
    
    
#else
    sscanf(in_string, "%d:%d", &hour,&minute);

#endif
    
    interval = convertToSeconds(hour,minute,0);

}

void Alert::getEvent(char *string) {
    sprintf(string, "%02d:%02d", hour,minute);
}

int Alert::getDependentDevice() {
    return dependentDeviceId;
}

void Alert::setDependentDevice(int id) {
    dependentDeviceId = id;
    if(dependentDeviceId > 0) {
        dependentDeviceObject = deviceDelegate.findDevice(dependentDeviceId);
    } else {
        dependentDeviceObject = NULL;
    }
}

char * Alert::getEmail() {
    //Serial.print("get email ");
    //Serial.println(email);
    return email;
}

void Alert::setEmail(char *in_email) {
    strcpy(email, in_email);
    //Serial.print("set email ");
    //Serial.println(email);
}

char * Alert::getSubject() {
    return subject;
}

void Alert::setSubject(char *in_subject) {
    strcpy(subject, in_subject);
}

char * Alert::getMessage() {
    return message;
}

void Alert::setMessage(char *in_message) {
    strcpy(message, in_message);
}


void Alert::serialize(JsonObject& doc)
{
    Device::serialize(doc);
     
    // Set the values in the document
    //doc["number"] = number;

    //doc["email"] = email;
    doc["subject"] = subject;
    doc["message"] = message;
    
    // 6 characters per event
    char event[6];
    getEvent(event);
    doc["event"] = event;

    doc["dependent"] = dependentDeviceId;
}

void Alert::deserialize(
    JsonObject& doc)
{
    Device::deserialize(doc);
    //if (doc.containsKey("email")) 
    //    strcpy(email, doc["email"])
    if (doc.containsKey("pin")) {
        pin = doc["pin"].as<int>();
        pinMode(pin, OUTPUT); // not handled in Device()
    }
    

    if (doc.containsKey("subject"))
        strcpy(subject, doc["subject"]);
    if (doc.containsKey("message"))
        strcpy(message, doc["message"]);

    // 6 characters per event
    char event[6];
    if (doc.containsKey("event")) {
        strcpy (event, doc["event"]);
        setEvent(event);
    }
        
    if (doc.containsKey("dependent")) {
         setDependentDevice(doc["dependent"].as<int>());
    }
       
    
}

