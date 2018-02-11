/*
 Video
*/
// © 2noodles llc
// minh@2noodles.com

#ifndef Video_h
#define Video_h

#include <Arduino.h>
#include "Device.h"

class Video : public Device
{
  public:
    Video(char *in_name, char *in_code);
     Video(char *in_name);
    ~Video() {}; // destructor
    
    bool isVideo() { return true; };

    void loop() {}; // required
    void getCode(char *inArray);
    char *getCode();
    void setCode(char *in_code);
        
    char code[20];
    
};//need ; at the end of a class def

#endif