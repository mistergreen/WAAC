/*
  Alert
*/

#include "Alert.h"
#include "../Device/Device.h"
#include "../DeviceDelegate/DeviceDelegate.h"
#include <EthernetClient.h>
#include <Time.h>
#include <WWWsettings.h>

static char server[] = "www.2noodles.com";
static char key[] = "greencontroller";

Alert::Alert(char *in_name, int in_dependent_device_id) : Device()
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
    //strcpy(server, "www.2noodles.com");
    //strcpy(key, "greencontroller");
    //setEmail(in_email);
    
    intervalTime = 0;
    interval = 0;
    
    //client = EthernetClient();
    
    //Alert has deviceState and methods
    //Serial.println("***email");
    // Serial.println(WWWsettings::toEmail);
}

Alert::~Alert() {
    //delete client;
    client = NULL;
}; // destructor


void Alert::loop()
{
    
    if(suspendTime == false) {
        
            if(dependentDeviceId > 0) { //if there's a dependent device
                //find dependent device - call DeviceDelegate
                boolean temp = dependentDeviceObject->getDeviceState();
                
                long currentTime = convertToSeconds(::hour(),::minute(),::second());
                
                if(temp == deviceState && intervalTime < currentTime ) {
                    
                    WWWsettings::globalEmail(getSubject(), getMessage());
                    intervalTime = interval + currentTime;
                } else if (temp != deviceState && intervalTime < currentTime) {
                    //don't reset right away
                    intervalTime = 0;
                }
            }
            
        
    }
}
/* uses system's emailer instead
void Alert::emailAlert()
{
    if (client.connect(server, 80)) {
        Serial.println("server connected");
        // Make a HTTP request:
        char emailString[150];
        sprintf(emailString, "to=%s&subject=%s&txt=%s&key=%s", WWWsettings::toEmail, subject, message, key);
        
        //Serial.println(emailString);
        
        client.print(F("GET /arduino_script/arduino_mail.php?"));
        //client.print(F("to=5137036979@vtext.com&subject=test&txt=testinmore&key=greencontroller"));
        client.print(emailString);
        
        client.println(F(" HTTP/1.1"));
        client.println(F("Host: www.2noodles.com"));
        client.println(F("Connection: close"));
        client.println();
    }
    else {
        // didn't get a connection to the server:
        Serial.println("connection failed");
    }
    
    if(client.connected()) {
        client.stop();
        Serial.println("disconnect from alert");
    }

    
}
*/

void Alert::setEvent(char *in_string) {
    
    if(in_string[0] == '\0') return;
    
    sscanf(in_string, "%d:%d", &hour,&minute);
    
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




