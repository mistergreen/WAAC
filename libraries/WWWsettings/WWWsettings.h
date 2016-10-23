/*
 WWWsettings
*/

#ifndef WWWsettings_h
#define WWWsettings_h

#include <EthernetClient.h>
#include <Arduino.h>

class WWWsettings
{
  public:
    WWWsettings();
    void begin();

    void check();
    boolean email(char *in_subject, char *in_message);
    //byte emailSMTP(char *in_subject, char *in_message);
    boolean updateDDNS();

    boolean waitForResponse(EthernetClient client);
    //byte emailResponse(EthernetClient client);
    
    long convertToSeconds(int in_hour, int in_minute, int in_second);
    
    void setNetworkIp(char *in_ip);
    char * getNetworkIp(char *in_holder);
    
    void setEvent(uint8_t hour, uint8_t minute);
    void getEvent(char *string);
    uint8_t getHour();
    uint8_t getMinute();
    
    char * getFromEmail();
    void setFromEmail(char *in_email);
    
    char * getToEmail();
    void setToEmail(char *in_email);
    
    char * getSubject();
    void setSubject(char *in_subject);
    
    char * getMessage();
    void setMessage(char *in_message);
    
    char * getUser();
    void setUser(char *in_user);
    
    char * getPassword();
    void setPassword(char *in_password);
    
    char * getSMTP();
    void setSMTP(char *in_smtp);
    
    int getPort();
    void setPort(int in_port);
    
    char * getNetworkAddress();
    void setNetworkAddress(char *in_address);
    
    char * getNetworkHost();
    void setNetworkHost(char *in_host);
    
    char * getDdnsHost();
    void setDdnsHost(char *in_host);
    
    char * getDdnsHostName();
    void setDdnsHostName(char *in_host);
    
    long getInterval();
    void setInterval(long in_time);
    
    void setEmailIpState(boolean in_state);
    boolean getEmailIpState();
    
    void setDDNSIpState(boolean in_state);
    boolean getDDNSIpState();
    
    
    char toEmail[50];
    char fromEmail[50];
    char subject[55];
    char smtp[50];
    int smtpPort;
    char user[50];
    char password[50];
    char message[200];
    char wwwType[30];
    
    char ddnsHost[50];
    char ddnsHostName[55];
    char networkIp[45];
    char oldNetworkIp[45];
    char networkAddress[45];
    char networkHost[45];
    
    long interval;
    long intervalTime;
    long previousTime;
    unsigned long waitTime;
    
    uint8_t hour;
    uint8_t minute;
    
    boolean isEmailIp;
    boolean isDDNSIp;
    


    
};
#endif