/*
 WWWsettings
*/
// © 2noodles llc
// minh@2noodles.com

#ifndef WWWsettings_h
#define WWWsettings_h

//#include <WiFiClient.h>
#include <Arduino.h>
#include <WiFiUdp.h>
#include <WiFi.h>
//#include <TimeLib.h>
#include <ezTime.h>
#include "Storable.h"

#define NTP_PACKET_SIZE 48

class WWWsettings: public Storable
{
  public:
    WWWsettings();

    // It resturns the static instance of the class.
    static WWWsettings* getinstance();

    // It initializes the class.
    void begin();

    // Class setup with parameters passing
    void begin(
        // The time zone to set.
        char * time_zone,
        // The ntp server name.
        char * ntp_server);

    void check();
   
    void email(char *in_subject, char *in_message);

    //byte emailSMTP(char *in_subject, char *in_message);
    boolean updateDDNS();

    boolean waitForResponse(NetworkClient client);
    //byte emailResponse(NetworkClient client);
    
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
    
    char * getNTPServer();
    void setNTPServer(char *in_ip);
    
    char* getTimeZone();
    void setTimeZone(char* zone);

    char* getLocalIP();
    char* getLocalGateway();
    char* getLocalSubMask();
    char* getLocalMacAddress();

    
    long getInterval();
    void setInterval(long in_time);
    
    void splitIP(char *inString, uint8_t *inArray);
    void syncNTP();
    void sendNTPpacket(byte *address);
    void sendNTPpacket(char *address);
    
    time_t getNtpTime();
    
    
    void setEmailIpState(boolean in_state);
    boolean getEmailIpState();
    
    void setDDNSIpState(boolean in_state);
    boolean getDDNSIpState();
    
    time_t testNTP();
    
    char * getToEmail();
    void setToEmail(char *in_email);

    char* getWiFiSSID();
    void setWiFiSSID(char* ssid);
    char* getWiFiPassword();
    void setWiFiPassword(char* password);

    
    static char toEmail[55];
    
    static WWWsettings *thisClassObj; // get instance to call pointer
    static time_t globalGetNTPTime() {
        return thisClassObj->getNtpTime();
    }
    static void globalEmail(char *in_subject, char *in_message) {
        thisClassObj->email(in_subject, in_message);
    }
    
    void setDayLightSaving(boolean val);
    uint8_t getDayLightSaving();

    // It serializes the class into a Json document.
    void serialize(
        // Input Json object pointer to be filled with the class information.
        JsonObject& doc);
    
    // It fills the class using the information contained into the document.
    void deserialize(
        // Input Json object pointer containing the class information.
        JsonObject& doc);

    // It returns the current time in use.
    Timezone* getTime();

    
  private:
   
    char fromEmail[50];
    char subject[55];
    char smtp[50];
    int smtpPort;
    char ddnsUser[50];
    char ddnsPassword[50];
    char message[200];
    
    char ddnsHost[50];
    char ddnsHostName[55];
    char networkIp[45];
    char oldNetworkIp[45];
    char networkAddress[45];
    char networkHost[45];

    char localIp[60];
    char localSubMask[60];
    char localGateway[60];
    char localMacAddress[60];

    char WiFi_SSID[60];
    char WiFi_password[60];
    
    long interval;
    long intervalTime;
    long previousTime;
    unsigned long waitTime;
    char timeZoneName[45];
    Timezone timeZone;
    uint8_t dayLightSaving;
    
    uint8_t eventHour;
    uint8_t eventMinute;
    
    boolean isEmailIp;
    boolean isDDNSIp;
    boolean syncOnce;
    
    NetworkClient client;
    
    WiFiUDP Udp;
    unsigned int localPort;  // local port to listen for UDP packets
    byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets
    byte timeServerIp[4];
    char NTPServer[55];

  
};
#endif