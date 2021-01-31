/*
 Video
*/
// © 2noodles llc
// minh@2noodles.com

#ifndef Video_h
#define Video_h

#include <Arduino.h>
#include "Device.h"
#include "Storable.h"

class Video : public Device, public Storable
{
  public:
    Video(char *in_name, char *in_code);
    Video(char *in_name = "");
    ~Video() {}; // destructor
    
    bool isVideo() { return true; };

    void loop() {}; // required
    void getCode(char *inArray);
    char *getCode();
    void setCode(char *in_code);

    // It serializes the class into a Json document.
    void serialize(
        // Input Json object pointer to be filled with the class information.
        JsonObject& doc);
    
    // It fills the class using the information contained into the document.
    void deserialize(
        // Input Json object pointer containing the class information.
        JsonObject& doc);
        
  private:
    char code[20];
    
};//need ; at the end of a class def

#endif