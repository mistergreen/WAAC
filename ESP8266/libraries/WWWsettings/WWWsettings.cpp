/*
  WWWsettings
*/
// © 2noodles llc
// minh@2noodles.com

#include "WWWsettings.h"
#include <rBase64.h>

//used for NTP sync function
WWWsettings* WWWsettings::thisClassObj;
//static member
char WWWsettings::toEmail[50] = {};

WWWsettings::WWWsettings()
{
    localPort = 8888;
    timeZone  = -5;
    strcpy(NTPip, "132.163.4.101"); //defaul NTP
    twoAM = convertToSeconds(18,0,0);
    syncOnce = false;
    //strcpy(toEmail, "");
    
}

void WWWsettings::begin()
{
 
    isDDNSIp = false;
    isEmailIp = false;
    intervalTime = 0;
    previousTime = 0;
    hour = 12;
    minute = 0;
    interval = 0;
    
    //NTPio get split up in timeServer[] - get sent to sendNTPacket
    splitIP(NTPip, timeServer);

    //timeServer(ipArray[0], ipArray[1], ipArray[2], ipArray[3]);
    //need to only do it once so you don't eat up all of the sockets
    Udp.begin(localPort);
    
}

void WWWsettings::syncNTP() {
    thisClassObj = this;
    Serial.println("waiting for sync");
    //sync expects a function, not method, so a static method is used
    setSyncProvider(globalGetNTPTime);
    setSyncInterval(twoAM);
    
}

void WWWsettings::splitIP(char *inString, uint8_t *inArray) {
    char *tokens;
    int i = 0;
    //make a copy to reassign later
    char temp[25];
    strcpy(temp, inString);
    
    tokens = strtok(inString, ".");
    while (tokens != NULL) {
        inArray[i] = atoi(tokens);
        tokens = strtok(NULL, ".");
        i++;
    }
    
    //reassembles NTPip
    strcpy(inString, temp);
    
}

time_t WWWsettings::testNTP() {

     return getNtpTime();
}

/*-------- NTP code ----------*/

time_t WWWsettings::getNtpTime()
{
    Udp.flush(); // discard any previously received packets
    Serial.println("Transmit NTP Request");
    sendNTPpacket(timeServer);
    uint32_t beginWait = millis();
 while (millis() - beginWait < 1500) {
    int size = Udp.parsePacket();
    if (size >= NTP_PACKET_SIZE) {
        Serial.println("Receive NTP Response");
        Udp.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
        unsigned long secsSince1900;
        // convert four bytes starting at location 40 to a long integer
        secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
        secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
        secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
        secsSince1900 |= (unsigned long)packetBuffer[43];
        return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
    }
 }
        Serial.println("No NTP Response :-(");
        return 0; // return 0 if unable to get the time

    
}

// send an NTP request to the time server at the given address
void WWWsettings::sendNTPpacket(byte *address)
{
    // set all bytes in the buffer to 0
    memset(packetBuffer, 0, NTP_PACKET_SIZE);
    // Initialize values needed to form NTP request
    // (see URL above for details on the packets)
    packetBuffer[0] = 0b11100011;   // LI, Version, Mode
    packetBuffer[1] = 0;     // Stratum, or type of clock
    packetBuffer[2] = 6;     // Polling Interval
    packetBuffer[3] = 0xEC;  // Peer Clock Precision
    // 8 bytes of zero for Root Delay & Root Dispersion
    packetBuffer[12]  = 49;
    packetBuffer[13]  = 0x4E;
    packetBuffer[14]  = 49;
    packetBuffer[15]  = 52;
    // all NTP fields have been given values, now
    // you can send a packet requesting a timestamp:
    Udp.beginPacket(address, 123); //NTP requests are to port 123
    Udp.write(packetBuffer, NTP_PACKET_SIZE);
    Udp.endPacket();
}



void WWWsettings::check()
{
   //**** add non blocking code
    //Serial.println("m ");
    //Serial.println(toEmail);
    long currentTime = convertToSeconds(::hour(),::minute(),::second());
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
    /*
     //don't need it. the tiem lib syncs for you with setSyncInterval(interval); in secs. Default is 5min?
    if(currentTime == twoAM && syncOnce == false) {
        //sync at 2am
        setSyncProvider(globalGetNTPTime);
        syncOnce = true;
    } else if(currentTime >= twoAM && syncOnce == true) {
        syncOnce = false;
    }
     */
    
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
        
        while (client.connected()) { //connected or data available
            if(client.available()) {
                char c = client.read();
                Serial.print(c);
            }
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
    
    sprintf(userpass, "%s:%s", user, password);
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
    hour = in_hour;
    minute = in_minute;
    
    interval = convertToSeconds(hour,minute,0);

}

uint8_t WWWsettings::getHour() {
    return hour;
}

uint8_t WWWsettings::getMinute() {
    return minute;
}

void WWWsettings::getEvent(char *string) {
    sprintf(string, "%02d:%02d", hour,minute);
}

//don't declare static in method cpp, only h
char * WWWsettings::getToEmail() {
    return WWWsettings::toEmail;
    Serial.println("---to email");
    Serial.println(WWWsettings::toEmail);
}

void WWWsettings::setToEmail(char *in_email) {
    strcpy(WWWsettings::toEmail, in_email);
    Serial.println("***to email");
    Serial.println(WWWsettings::toEmail);
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

char * WWWsettings::getNTPip() {
    return NTPip;
    
}

void WWWsettings::setNTPip(char *in_ip){
     //strcpy(NTPip, in_ip);
    //setup and copy
    splitIP(NTPip, timeServer);
}

int WWWsettings::getTimeZone() {
    return timeZone;
}

void WWWsettings::setTimeZone(int zone){
    timeZone = zone;
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
    return user;
}

void WWWsettings::setUser(char *in_user){
    strcpy(user, in_user);
}

char * WWWsettings::getPassword(){
    return password;
}

void WWWsettings::setPassword(char *in_password){
    strcpy(password, in_password);
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





