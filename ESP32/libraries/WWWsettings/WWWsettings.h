/*
 WWWsettings
*/
// © 2noodles llc
// minh@2noodles.com

#ifndef WWWsettings_h
#define WWWsettings_h

#include <WiFiClient.h>
#include <Arduino.h>
#include <WiFiUdp.h>
#include <TimeLib.h>
#include "Storable.h"

#define NTP_PACKET_SIZE 48

class WWWsettings: public Storable
{
  public:
    WWWsettings();
    void begin();

    // Class setup with parameters passing
    void begin(
        // The time zone to set.
        int time_zone,
        // The ntp server name.
        char *ntp_server);

    void check();
   
    void email(char *in_subject, char *in_message);
    //byte emailSMTP(char *in_subject, char *in_message);
    boolean updateDDNS();

    boolean waitForResponse(WiFiClient client);
    //byte emailResponse(WiFiClient client);
    
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
    
    int getTimeZone();
    void setTimeZone(int zone);
    
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

    uint8_t* getLocalIP();
    void setLocalIP(uint8_t ip1, uint8_t ip2, uint8_t ip3, uint8_t ip4);

    uint8_t* getLocalSubnet();
    void setLocalSubnet(uint8_t sn1, uint8_t sn2, uint8_t sn3, uint8_t sn4);

    uint8_t* getLocalGW();
    void setLocalGW(uint8_t gw1, uint8_t gw2, uint8_t gw3, uint8_t gw4);

    char* getWiFiSSID();
    void setWiFiSSID(char* ssid);

    char* getWiFiPassword();
    void setWiFiPassword(char* password);

    // It serializes the class into a Json document.
    void serialize(
        // Input Json object pointer to be filled with the class information.
        JsonObject& doc);
    
    // It fills the class using the information contained into the document.
    void deserialize(
        // Input Json object pointer containing the class information.
        JsonObject& doc);
    
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
    
    long interval;
    long intervalTime;
    long previousTime;
    unsigned long waitTime;
    long twoAM;
    int timeZone;
    uint8_t dayLightSaving;
    
    uint8_t hour;
    uint8_t minute;
    
    boolean isEmailIp;
    boolean isDDNSIp;
    boolean syncOnce;
    
    WiFiClient client; //take the last 4th socket
    
    WiFiUDP Udp;
    unsigned int localPort;  // local port to listen for UDP packets
    byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets
    byte timeServerIp[4];
    char NTPServer[55];

    // The local IP adrress.
    uint8_t localIP[4];

    // The local subnet mask.
    uint8_t localSub[4];
    
    // The local gateway.
    uint8_t localGW[4];

    // The WiFi SSID.
    char WiFi_SSID[32];

    // The WiFi password.
    char WiFi_password[64];
};
#endif