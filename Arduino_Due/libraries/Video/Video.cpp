/*
  Video
*/
// © 2noodles llc
// minh@2noodles.com

#include "Video.h"
#include "Device.h"

Video::Video(char *in_name, char *in_code) : Device()
{
    //deviceID is automatically set my deviceDeleGate

    strcpy(deviceName,in_name);
    //classType inherit from base
    strcpy(classType,"Video");
    
    strcpy(code,in_code);
   
    
}

char *Video::getCode() {
    return code;
    
}

void Video::setCode(char *in_code) {
    strcpy(code,in_code);
    
}




