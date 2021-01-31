/*
  Video
*/
// © 2noodles llc
// minh@2noodles.com


#include "Video.h"
#include "Device.h"

Video::Video(char *in_name, char *in_code) : Device(), Storable()
{
    //deviceID is automatically set my deviceDeleGate
    //crashes, maybe format the code = {'\0'} ?
    strcpy(deviceName,in_name);
    //classType inherit from base
    strcpy(classType,"Video");
    
    strcpy(code,in_code);
    pin = 0;
    
}

Video::Video(char *in_name) : Device(), Storable()
{
    //deviceID is automatically set my deviceDeleGate
    
    strcpy(deviceName,in_name);
    //classType inherit from base
    strcpy(classType,"Video");
    pin = 0;
    
}


void Video::getCode(char *inArray) {
    strcpy(inArray, code);
    
}

char *Video::getCode() {
    return code;
    
}

void Video::setCode(char *in_code) {
     // junk character at the end
    code[20] = {'\0'};
    strcpy(code,in_code);
    
}

void Video::serialize(JsonObject& doc)
{
    // First call father serialization
    Device::serialize(doc);
    
    doc["code"] = code;
}

void Video::deserialize(
    JsonObject& doc)
{
   // First call father deserialization
    Device::deserialize(doc);

    strcpy(code, doc["code"]);
}


