/*
 Alert
*/
#ifndef Alert_h
#define Alert_h

#include "../Device/Device.h" 
#include <EthernetClient.h>

class Alert : public Device
{
  public:
    Alert(char *in_name, int in_dependent_device_id);
    ~Alert(); // destructor

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
    
    EthernetClient client;
    
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


    
};
#endif