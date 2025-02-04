// © 2noodles llc
// minh@2noodles.com


#ifndef Storable_h
#define Storable_h

#include "ArduinoJson.h"

// Class that defines the storable object functions.
class Storable
{
  public:
    
    // It serializes the class into a Json document.
    virtual void serialize(
    // Input Json object pointer to be filled with the class information.
        JsonObject& doc) = 0;
    
    // It fills the class using the information contained into the document.
    virtual void deserialize(
        // Input Json object pointer containing the class information.
        JsonObject& doc) = 0;
};

#endif