/*
 relay
*/

// © 2noodles llc
// minh@2noodles.com

#ifndef RelayPCA_h
#define RelayPCA_h

#include "Arduino.h"
#include "Relay.h"
#include "Storable.h"
#include "Adafruit_PWMServoDriver.h"

class RelayPCA : public Relay
{
  public:
      // Empty constructor.
    RelayPCA();
    RelayPCA(char *in_name, int in_pin, int in_dependent_device_id);
    ~RelayPCA() {}; // destructor
    
    // It serializes the class into a Json document.
    void serialize(
    // Input Json object pointer to be filled with the class information.
        JsonObject& doc);
    
    // It fills the class using the information contained into the document.
    void deserialize(
        // Input Json object pointer containing the class information.
        JsonObject& doc);

    virtual void switchOn();
    virtual void switchOff();
    void getI2C(int *inArray);
    void setI2C(int insda, int inscl);
    
  private:

    int SDA;
    int SCL;
    Adafruit_PWMServoDriver pwmObj; // automatic memory

    
};//need ; at the end of a class def

#endif