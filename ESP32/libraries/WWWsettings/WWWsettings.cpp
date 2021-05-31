/*
  WWWsettings
 
 X-need to code for ntp to accept nameserver too!
 easier just to use servername instead of both name & IP
*/

// © 2noodles llc
// minh@2noodles.com

#include "WWWsettings.h"
#include <rBase64.h>

//used for NTP sync function
WWWsettings* WWWsettings::thisClassObj = NULL;
//static member
char WWWsettings::toEmail[55] = {};

WWWsettings* WWWsettings::getinstance()
{
    if (thisClassObj == NULL)
    {
        thisClassObj = new WWWsettings();
    }

    return thisClassObj;
}

WWWsettings::WWWsettings()
{
    localPort = 8888;

    syncOnce = false;
    //strcpy(toEmail, "");
    dayLightSaving = 0;

    // The local IP adrress.
    memset(localIP, 0, sizeof(localIP));

    // The local subnet mask.
    memset(localSub, 0, sizeof(localSub));
    
    // The local gateway.
    memset(localGW, 0, sizeof(localGW));

    // The WiFi SSID.
    strcpy(WiFi_SSID,"");

    // The WiFi password.
    strcpy(WiFi_password,"");
    
}

void WWWsettings::begin()
{
 
    isDDNSIp = false;
    isEmailIp = false;
    intervalTime = 0;
    previousTime = 0;
    eventHour = 12;
    eventMinute = 0;
    interval = 0;
    
    //NTPio get split up in timeServerIp[] - get sent to sendNTPacket
    //splitIP(NTPServer, timeServerIp);

    //timeServerIp(ipArray[0], ipArray[1], ipArray[2], ipArray[3]);
    //need to only do it once so you don't eat up all of the sockets
    Udp.begin(localPort);

    // Wait time to sync.
    waitForSync();
}

void WWWsettings::begin(
    // The time zone to set.
    char * time_zone,
    // The ntp server name.
    char *ntp_server) {
        setNTPServer(ntp_server);
        setTimeZone (time_zone);
        isDDNSIp = false;
        isEmailIp = false;
        intervalTime = 0;
        previousTime = 0;
        eventHour = 12;
        eventMinute = 0;
        interval = 0;
        
        //timeServerIp(ipArray[0], ipArray[1], ipArray[2], ipArray[3]);
        //need to only do it once so you don't eat up all of the sockets
        Udp.begin(localPort);
    }

void WWWsettings::syncNTP() {
    // Set again the location, it network was missing the location may not be properly set.
    timeZone.setLocation(timeZoneName);
    updateNTP();
}

void WWWsettings::splitIP(char *inString, uint8_t *inArray) {
    char *tokens;
    int i = 0;
    //make a copy to reassign later
    char temp[55];
    strcpy(temp, inString);
    
    tokens = strtok(inString, ".");
    while (tokens != NULL) {
        inArray[i] = atoi(tokens);
        tokens = strtok(NULL, ".");
        i++;
    }
    
    //reassembles NTPServer
    strcpy(inString, temp);
    
}

time_t WWWsettings::testNTP() {

    updateNTP();

    //time_t time = getNtpTime();
    //setTime(time);
    return now();
}


void WWWsettings::check()
{
   //**** add non blocking code
    //Serial.println("m ");
    //Serial.println(toEmail);


    long currentTime = convertToSeconds(timeZone.hour(), timeZone.minute(), timeZone.second());
   // Serial.println(currentTime);
    //Serial.println(interval);
    
    if(interval > 0) { // only go through if interval is set which is alway

        //check for interval
        //check every second intervalTime, 12 hours as default
        if(currentTime != previousTime) {
            
            previousTime = currentTime;
            if(intervalTime < currentTime) {
                intervalTime = interval + currentTime;

              
                if(isDDNSIp) {
                    
                    updateDDNS();
                    
                } else if(isEmailIp) {
                    //get IP
                    char tempIp[45];
                    strcpy(oldNetworkIp, getNetworkIp(tempIp));
                    
                    if(strcmp(oldNetworkIp,networkIp) != 0) {
                        strcpy(networkIp, oldNetworkIp);
                        memset(&message[0], 0, sizeof(message));
                        sprintf(message, "Your new IP is %s", networkIp);
                        strcpy(subject, "Arduino IP");
                        email(subject, message);
                    }

                }
               
                
            }//interval
           
        }//every second
        
    }// interval exists
    
    if (client.available()) {
        char c = client.read();
        Serial.print(c);
    }
    
    if (!client.connected()) {
        //Serial.println();
        //Serial.println("disconnecting.");
        client.stop();

    }

}


void WWWsettings::email(char *in_subject, char *in_message) {
//Serial.println("2noodles mail start");
    //WiFiClient client;
    
    if (client.connect("www.2noodles.com", 80)) {
        
       // Make a HTTP request:
        char emailString[200];
        char key[] = "greencontroller";
        sprintf(emailString, "to=%s&subject=%s&txt=%s&key=%s", WWWsettings::toEmail, in_subject, in_message, key);
        
        //post
        client.println("POST /arduino_script/arduino_mail.php HTTP/1.1");
        client.println("Host: www.2noodles.com");
        client.println("Connection: close");
        client.println("Content-Type: application/x-www-form-urlencoded");
        client.print("Content-Length: ");
        client.println(strlen(emailString));
        client.println();
        client.println(emailString);
        
    } else {
            // didn't get a connection to the server:
            Serial.println("connection failed");
            client.println();
            
    }
    /*    
    while (client.connected()) { //connected or data available
        if(client.available()) {
            char c = client.read();
            Serial.print(c);
        }
    }
    */

    waitTime = millis();
    while (!client.available())
    {
        if((millis() - waitTime) > 10000) {
            client.stop();
            Serial.println("\r\n email Timeout");
            break;
           
        }
    }


    while (client.available())
    {
        char c = client.read();
        Serial.print(c);

    }
   
    Serial.println("disconnecting.");
    
    
    client.stop(); // close up after disconnect the socket

}

void WWWsettings::setNetworkIp(char *in_ip) {
    strcpy(networkIp, in_ip);
}

char * WWWsettings::getNetworkIp(char *in_holder) {
    
    //WiFiClient client;
    Serial.println("getting network ip");
    if (client.connect(networkAddress, 80))
    {
        Serial.println("ip connected");
        client.println("GET / HTTP/1.0");
        client.print("HOST: ");
        client.println(networkHost);
        client.println();
    } else {
        Serial.println("connection failed");
        client.stop();
        return 0;
        //retrun '\0' works too.
    }
    
    waitTime = millis();
    while (!client.available())
    {
        if((millis() - waitTime) > 10000) {
            client.stop();
            Serial.println("\r\nIp Timeout");
            return 0;
        }
    }
    
    char temp[400];
    uint8_t i = 0;
    while (client.available())
    {
        char c = client.read();
        temp[i] = c;
        //Serial.print(c);
        i++;
        
    }//end while
    temp[i] = '\0';
    Serial.println(temp);
    //client.flush();
    
    
    //Strip IP
    //Find :  Current IP Address: 184.59.87.241
    // or Find \r\n\r\n
    if(strlen(temp) > 5) {
        
        char * pch;
        int val_len = 0;
        if(pch = strstr(temp,"Address:")) {
            pch +=9;
        
        } else if (pch = strstr(temp, "\r\n\r\n")) {
            pch +=4;
           
        }
       
        char *s = pch;
        char *value_start_pos = s;
        
        while(*s != '\n' && *s != '\0' && *s != ' ') {
            val_len++;
            s++;
            
        }
        
        strncpy(in_holder, value_start_pos, val_len);
        in_holder[val_len+1] = '\0';
        //Serial.println("xxxxx");
        //Serial.println(in_holder);
        client.stop();
        return in_holder;

    }
    
    if (!client.connected())
    {
        
        Serial.println(F("disconnecting."));
        client.stop();
        return 0;
        
    }
}

boolean WWWsettings::updateDDNS() {
    Serial.println("update ddns");
    
    char userpass[100]={'\0'};
    
    sprintf(userpass, "%s:%s", ddnsUser, ddnsPassword);
    //Serial.println(userpass);
    String output = rbase64.encode(userpass);
    //Serial.println(output);
    if (client.connect(ddnsHost, 80))
    {
        Serial.println("ddns connected");
        client.print("GET /nic/update?hostname=");
        client.print(ddnsHostName);
        client.println(" HTTP/1.0");
        client.print("Host: ");
        client.println(ddnsHost);
        client.print("Authorization: Basic ");
        client.println(output);
        client.println("User-Agent: WAAC Controller/1.0 minh@2noodles.com");
        client.println();
        
    } else {
        Serial.println("ddns failed");
        client.stop();
        return false;
    }
    // response
    char response[400] = {'\0'};
    
    waitTime = millis();
    while (!client.available())
    {
        
        if((millis() - waitTime) > 7000) {
            client.stop();
            Serial.println("\r\nTimeout");
            return false;
        }
    }
    int i = 0;
    while (client.available())
    {
        char c = client.read();
        response[i++] = c;
        
    }//end while
    Serial.println(response);
    if(strstr(response,"200 OK")) {
        return true;
    } else {
        return false;
    }
    
}



void WWWsettings::setPort(int in_port)
{
    smtpPort = in_port;
}

int WWWsettings::getPort()
{
    return smtpPort;
}

void WWWsettings::setSMTP(char *in_smtp)
{
    strcpy(smtp, in_smtp);
}

char * WWWsettings::getSMTP()
{
    return smtp;
}


void WWWsettings::setEvent(uint8_t in_hour, uint8_t in_minute) {
    eventHour = in_hour;
    eventMinute = in_minute;
    
    interval = convertToSeconds(eventHour,eventMinute,0);

}

uint8_t WWWsettings::getHour() {
    return eventHour;
}

uint8_t WWWsettings::getMinute() {
    return eventMinute;
}

void WWWsettings::getEvent(char *string) {
    sprintf(string, "%02d:%02d", eventHour,eventMinute);
}

//don't declare static in method cpp, only h
char * WWWsettings::getToEmail() {
    return WWWsettings::toEmail;
    //Serial.println("---to email");
    //Serial.println(WWWsettings::toEmail);
}

void WWWsettings::setToEmail(char *in_email) {
    strcpy(WWWsettings::toEmail, in_email);
    //Serial.println("***to email");
    //Serial.println(WWWsettings::toEmail);
}

char * WWWsettings::getFromEmail() {
    return fromEmail;
}

void WWWsettings::setFromEmail(char *in_email) {
    strcpy(fromEmail, in_email);
}


char * WWWsettings::getNetworkAddress() {
    return networkAddress;
}

void WWWsettings::setNetworkAddress(char *in_address) {
    strcpy(networkAddress, in_address);
}

char * WWWsettings::getNetworkHost() {
    return networkHost;
}

void WWWsettings::setNetworkHost(char *in_host) {
    strcpy(networkHost, in_host);
   
}

char * WWWsettings::getNTPServer() {
    return NTPServer;
    
}

void WWWsettings::setNTPServer(char *in_ip){
    strcpy(NTPServer, in_ip);

    setServer(NTPServer);

    //setup and copy-  get rid of ip
    //splitIP(NTPServer, timeServerIp);
}

char * WWWsettings::getTimeZone() {
    return timeZoneName;
}

void WWWsettings::setTimeZone(char * zone){
    strncpy(timeZoneName, zone, 45);
    timeZone.setLocation(timeZoneName);
}

Timezone* WWWsettings::getTime(){
    return &timeZone;
}


char * WWWsettings::getDdnsHost() {
    return ddnsHost;
}

void WWWsettings::setDdnsHost(char *in_host) {
    strcpy(ddnsHost, in_host);
}

char * WWWsettings::getDdnsHostName() {
    return ddnsHostName;
}

void WWWsettings::setDdnsHostName(char *in_host) {
    strcpy(ddnsHostName, in_host);
}


char * WWWsettings::getSubject() {
    return subject;
}

void WWWsettings::setSubject(char *in_subject) {
    strcpy(subject, in_subject);
}

char * WWWsettings::getMessage() {
    return message;
}

void WWWsettings::setMessage(char *in_message) {
    strcpy(message, in_message);
}

long WWWsettings::convertToSeconds(int in_hour, int in_minute, int in_second) {
    return in_hour*3600L +in_minute*60L +in_second;
}

char * WWWsettings::getUser(){
    return ddnsUser;
}

void WWWsettings::setUser(char *in_user){
    strcpy(ddnsUser, in_user);
}

char * WWWsettings::getPassword(){
    return ddnsPassword;
}

void WWWsettings::setPassword(char *in_password){
    strcpy(ddnsPassword, in_password);
}

void WWWsettings::setEmailIpState(boolean in_state) {
    isEmailIp = in_state;
    if(isEmailIp) {
        isDDNSIp = false;
        
    }
}
boolean WWWsettings::getEmailIpState() {
    return isEmailIp;
}

void WWWsettings::setDDNSIpState(boolean in_state){
    isDDNSIp = in_state;
    
    if(isDDNSIp) {
        isEmailIp = false;
    }
}

boolean WWWsettings::getDDNSIpState() {
    return isDDNSIp;
}

void WWWsettings::setDayLightSaving(boolean val) {
    if(val)
        dayLightSaving = 1;
    else
        dayLightSaving = 0;
    
}

uint8_t WWWsettings::getDayLightSaving() {
    
    return dayLightSaving;
}


/*
byte WWWsettings::emailResponse(WiFiClient client)
{
    waitTime = millis();
    while (!client.available())
    {

        if((millis() - waitTime) > 10000) {
            client.stop();
            Serial.println(F("\r\nTimeout"));
            return 0;
        }
    }
    
    while (client.available())
    {
        char c = client.read();
        Serial.print(c);
    }//end while
    //Serial.println();
    return 1;
    
}
 */


boolean WWWsettings::waitForResponse(WiFiClient client)
{
    waitTime = millis();
    while (!client.available())
    {
        
        if((millis() - waitTime) > 10000) {
            client.stop();
            Serial.println("\r\nTimeout");
            return false;
        }
    }
    
    while (client.available())
    {
        char c = client.read();
        Serial.print(c);
    }//end while
    
    return true;
}


uint8_t* WWWsettings::getLocalIP()
{
    return localIP;
}

void WWWsettings::setLocalIP(uint8_t ip1, uint8_t ip2, uint8_t ip3, uint8_t ip4)
{
    Serial.print("Setting IP ");
    Serial.print(ip1);
    Serial.print(".");
    Serial.print(ip2);
    Serial.print(".");
    Serial.print(ip3);
    Serial.print(".");
    Serial.println(ip4);

    localIP[0] = ip1;
    localIP[1] = ip2;
    localIP[2] = ip3;
    localIP[3] = ip4;
}

uint8_t* WWWsettings::getLocalSubnet()
{
    return localSub;
}

void WWWsettings::setLocalSubnet(uint8_t sn1, uint8_t sn2, uint8_t sn3, uint8_t sn4)
{
    Serial.print("Setting Subnet ");
    Serial.print(sn1);
    Serial.print(".");
    Serial.print(sn2);
    Serial.print(".");
    Serial.print(sn3);
    Serial.print(".");
    Serial.println(sn4);
    
    localSub[0] = sn1;
    localSub[1] = sn2;
    localSub[2] = sn3;
    localSub[3] = sn4;
}

uint8_t* WWWsettings::getLocalGW()
{
    return localGW;
}

void WWWsettings::setLocalGW(uint8_t gw1, uint8_t gw2, uint8_t gw3, uint8_t gw4)
{
    localGW[0] = gw1;
    localGW[1] = gw2;
    localGW[2] = gw3;
    localGW[3] = gw4;
}

char* WWWsettings::getWiFiSSID()
{
    return WiFi_SSID;
}

void WWWsettings::setWiFiSSID(char* ssid)
{
    strncpy(WiFi_SSID, ssid, 32);
}

char* WWWsettings::getWiFiPassword()
{
    return WiFi_password;
}

void WWWsettings::setWiFiPassword(char* password)
{
    strncpy(WiFi_password, password, 64);
}


void WWWsettings::serialize(JsonObject& doc)
{
    doc["fromEmail"] = fromEmail;
    doc["subject"] = subject;
    doc["smtp"] = smtp;
    doc["smtpPort"] = smtpPort;
    doc["ddnsUser"] = ddnsUser;
    doc["ddnsPassword"] = ddnsPassword;
    doc["message"] = message;
    doc["ddnsHost"] = ddnsHost;
    doc["ddnsHostName"] = ddnsHostName;

    doc["networkIp"] = networkIp;
    doc["oldNetworkIp"] = oldNetworkIp;
    doc["networkAddress"] = networkAddress;
    doc["networkHost"] = networkHost;
    
    doc["timeZone"] = timeZoneName;
    doc["dayLightSaving"] = dayLightSaving;
    
    doc["hour"] = eventHour;
    doc["minute"] = eventMinute;

    doc["isEmailIp"] = isEmailIp;
    doc["isDDNSIp"] = isDDNSIp;

    doc["NTPServer"] = NTPServer;

    // The local IP adrress.
    doc["localIP0"] = localIP[0];
    doc["localIP1"] = localIP[1];
    doc["localIP2"] = localIP[2];
    doc["localIP3"] = localIP[3];

    // The local subnet mask.
    doc["localSub0"] = localSub[0];
    doc["localSub1"] = localSub[1];
    doc["localSub2"] = localSub[2];
    doc["localSub3"] = localSub[3];
    
    // The local gateway.
    doc["localGW0"] = localGW[0];
    doc["localGW1"] = localGW[1];
    doc["localGW2"] = localGW[2];
    doc["localGW3"] = localGW[3];

    // The WiFi SSID.
    doc["WiFi_SSID"] = WiFi_SSID;

    // The WiFi password.
    doc["WiFi_password"] = WiFi_password;
}

void WWWsettings::deserialize(
    JsonObject& doc)
{
    strcpy(fromEmail, doc["fromEmail"]);
    strcpy(subject, doc["subject"]);
    strcpy(smtp, doc["smtp"]);
    smtpPort = doc["smtpPort"];
    strcpy(ddnsUser, doc["ddnsUser"]);
    strcpy(ddnsPassword, doc["ddnsPassword"]);
    strcpy(message, doc["message"]);
    strcpy(ddnsHost, doc["ddnsHost"]);
    strcpy(ddnsHostName, doc["ddnsHostName"]);

    strcpy(networkIp, doc["networkIp"]);
    strcpy(oldNetworkIp, doc["oldNetworkIp"]);
    strcpy(networkAddress, doc["networkAddress"]);
    strcpy(networkHost, doc["networkHost"]);

    strcpy(timeZoneName, doc["timeZone"]);

    dayLightSaving = doc["dayLightSaving"];
    
    eventHour = doc["hour"];
    eventMinute = doc["minute"];

    isEmailIp = doc["isEmailIp"];
    isDDNSIp = doc["isDDNSIp"];

    strcpy(NTPServer, doc["NTPServer"]);

    // The local IP adrress.
    localIP[0] = doc["localIP0"];
    localIP[1] = doc["localIP1"];
    localIP[2] = doc["localIP2"];
    localIP[3] = doc["localIP3"];

    // The local subnet mask.
    localSub[0] = doc["localSub0"];
    localSub[1] = doc["localSub1"];
    localSub[2] = doc["localSub2"];
    localSub[3] = doc["localSub3"];

    // The local gateway.
    localGW[0] = doc["localGW0"];
    localGW[1] = doc["localGW1"];
    localGW[2] = doc["localGW2"];
    localGW[3] = doc["localGW3"];

    // The WiFi SSID.
    strcpy(WiFi_SSID, doc["WiFi_SSID"]);

    // The WiFi password.
    strcpy(WiFi_password, doc["WiFi_password"]);

    setServer(NTPServer);

    timeZone.setLocation(timeZoneName);
}



