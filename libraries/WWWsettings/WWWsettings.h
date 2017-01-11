/*
 WWWsettings
*/

#ifndef WWWsettings_h
#define WWWsettings_h

#include <EthernetClient.h>
#include <Arduino.h>
#include <EthernetUdp.h>
#include <Time.h>

#define NTP_PACKET_SIZE 48

class WWWsettings
{
  public:
    WWWsettings();
    void begin();

    void check();
   
    void email(char *in_subject, char *in_message);
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
    
    char * getNTPip();
    void setNTPip(char *in_ip);
    
    int getTimeZone();
    void setTimeZone(int zone);
    
    long getInterval();
    void setInterval(long in_time);
    
    void splitIP(char *inString, uint8_t *inArray);
    void syncNTP();
    void sendNTPpacket(byte *address);
    time_t getNtpTime();
    
    
    void setEmailIpState(boolean in_state);
    boolean getEmailIpState();
    
    void setDDNSIpState(boolean in_state);
    boolean getDDNSIpState();
    
    time_t testNTP();
    
    char * getToEmail();
    void setToEmail(char *in_email);
    
    static char toEmail[50];
    
    static WWWsettings *thisClassObj; // get instance to call pointer
    static time_t globalGetNTPTime() {
        return thisClassObj->getNtpTime();
    }
    static void globalEmail(char *in_subject, char *in_message) {
        thisClassObj->email(in_subject, in_message);
    }
    

    
    
  private:
   
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
    char NTPip[25];
    
    long interval;
    long intervalTime;
    long previousTime;
    unsigned long waitTime;
    long twoAM;
    int timeZone;
    
    uint8_t hour;
    uint8_t minute;
    
    boolean isEmailIp;
    boolean isDDNSIp;
    boolean syncOnce;
    
    EthernetClient client; //take the last 4th socket
    
    EthernetUDP Udp;
    unsigned int localPort;  // local port to listen for UDP packets
    byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets
    byte timeServer[4];
    
   

    
};
#endif