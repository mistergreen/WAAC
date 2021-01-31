/*
 Alert
*/

// © 2noodles llc
// minh@2noodles.com


#ifndef Alert_h
#define Alert_h

#include "Device.h"
#include "Storable.h"

class Alert : public Device, public Storable
{
  public:
    // Empty constructor for deserialization.
    Alert();

    Alert(char *in_name, int in_dependent_device_id);
    ~Alert(); // destructor
    
    // It serializes the class into a Json document.
    void serialize(
    // Input Json object pointer to be filled with the class information.
        JsonObject& doc);
    
    // It fills the class using the information contained into the document.
    void deserialize(
        // Input Json object pointer containing the class information.
        JsonObject& doc);

    void loop(); // required
    
    bool isAlert() { return true; };
    
    void setEvent(char *in_string);
    void getEvent(char *string);
    
    int getDependentDevice();
    void setDependentDevice(int id);
    
    Device *dependentDeviceObject;
    
    void emailAlert();
    
    char * getEmail();
    void setEmail(char *in_email);
    
    char * getSubject();
    void setSubject(char *in_subject);
    
    char * getMessage();
    void setMessage(char *in_message);
    
    
    char email[35];
    char subject[35];
    char message[75];
    
    int dependentDeviceId;
    uint8_t hour;
    uint8_t minute;
    
  private:
    //char server[35];
    //char key[35]; //needed to email, like a password
    long interval;
    long intervalTime;

    // It suspends the events checking.
    bool suspendTime;
};
#endif