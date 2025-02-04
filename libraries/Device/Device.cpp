// © 2noodles llc
// minh@2noodles.com

//Base Class
//never use by itself. It needs to be included in a device class

#include "Arduino.h"
#include "Device.h"


const int Device::NUM_BUTTON_ACTIONS = 1;

const char* Device::NAME_BUTTON_ACTIONS = "Toggle State";

Device::Device()
{
    deviceState = false;
    
    strcpy(imageName,"none.jpg");
    strcpy(deviceColor,"FFFFFF");

    min = 0.0;
    max = 100.0;
    strcpy(unit,"");
}

void Device::setMin(float in_min) {
    min = in_min;
}

float Device::getMin() {
    return min;
}

void Device::setMax(float in_max) {
    max = in_max;
}

float Device::getMax() {
    return max;
}

void Device::setUnit(char *in_unit) {
    strcpy(unit, in_unit);
}


char * Device::getUnit() {
    return unit;
    
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

bool Device::getDeviceState() {
    return deviceState;
}

void Device::setDeviceState(bool state) {
    deviceState = state;
}


int Device::getNumButtonActions()
{
    return NUM_BUTTON_ACTIONS;
}


const char* Device::getButtonActionName(
    int actionId)
{
    // The returned value.
    char* retVal = "";

    // Check the id before parforming any action.
    if (actionId == 0)
    {
        retVal = (char *) NAME_BUTTON_ACTIONS;
    }

    return retVal;
}

void Device::callButtonAction(
    int actionId)
{
    // Check the id before parforming any action.
    if (actionId == 0)
    {
        Serial.print("Device::callButtonAction calling action ");
        Serial.println(actionId);
        toggleState();
    }
}


void Device::serialize(JsonObject& doc)
{
    // Set the values in the document
    //doc["number"] = number;
    doc["pin"] = pin;
    doc["classType"] = classType;
    doc["deviceName"] = deviceName;
    doc["imageName"] = imageName;
    doc["deviceColor"] = deviceColor;
    doc["deviceState"] = deviceState;
    doc["deviceId"] = deviceId;
    doc["min"] = min;
    doc["max"] = max;
    doc["unit"] = unit;
}

void Device::deserialize(JsonObject& doc)
{
    //make sure it's correct type & key exist (don't evaluate doc[key] where it's null) or esp will crash
    if (doc.containsKey("pin"))
        pin = doc["pin"].as<int>();

    if (doc.containsKey("deviceType"))
        strcpy(classType, doc["deviceType"]);

    if (doc.containsKey("deviceName"))
        strcpy(deviceName, doc["deviceName"]);
    if (doc.containsKey("deviceColor"))
        strcpy(deviceColor, doc["deviceColor"]);
    if (doc.containsKey("imageName"))
        strcpy(imageName, doc["imageName"]);

    if (doc.containsKey("deviceState"))
        deviceState = doc["deviceState"].as<bool>();

    // don't assign deviceID if creating from browser
    if (doc.containsKey("deviceId"))   
        deviceId = doc["deviceId"].as<int>();

    if (doc.containsKey("min"))       
        min = doc["min"].as<float>();
    if (doc.containsKey("max"))
        max = doc["max"].as<float>();
    if (doc.containsKey("unit"))
        strcpy(unit, doc["unit"]);
}
