// © 2noodles llc
// minh@2noodles.com

//Base Class
//never use by itself. It needs to be included in a device class

#include "Arduino.h"
#include "Device.h"


Device::Device()
{
    
    deviceState = false;
    suspendTime = false;
    
    strcpy(imageName,"none.jpg");
    strcpy(deviceColor,"FFFFFF");
 
}

void Device::stripTime(char *inString, int *inArray) {
    char *tok1;
    int i = 0;
    tok1 = strtok(inString, ":");
    while (tok1 != NULL) {
        inArray[i] = atoi(tok1);
        tok1 = strtok(NULL, ":");
        i++;
        
    }
}


long Device::convertToSeconds(int in_hour, int in_minute, int in_second) {
    return in_hour*3600L +in_minute*60L +in_second;
}

char * Device::getClassName() {
    return classType;
}

//----------- image & color
void Device::setStyle(char *image, char *color) {
    strcpy(imageName,image);
    strcpy(deviceColor,color);
}

void Device::setImageName(char *image) {
    strcpy(imageName,image);
}

void Device::setDeviceColor(char *color) {
    strcpy(deviceColor,color);
}

char * Device::getImageName() {
    return imageName;
}

char * Device::getDeviceColor() {
    return deviceColor;
}

//-----------
char * Device::getDeviceName() {
    return deviceName;
}

void Device::setDeviceName(char *name) {
     strcpy(deviceName,name);
    
}
//-----------
int Device::getPin() {
    return pin;
}

void Device::setPin(int in_pin) {
    pin = in_pin;
}
//-----------
void Device::setDeviceId(int id) {
    deviceId = id;
}

int Device::getDeviceId() {
    return deviceId;
    
}
//-----------

void Device::setSuspendTime(boolean in_suspend) {
    suspendTime = in_suspend;
    
}

boolean Device::getSuspendTime() {
    return suspendTime;
    
}

//-----------

boolean Device::getDeviceState() {
    return deviceState;
}

void Device::setDeviceState(bool state) {
    deviceState = state;
}

