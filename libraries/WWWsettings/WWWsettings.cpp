/*
  WWWsettings
*/

#include "WWWsettings.h"
#include <Time.h>
#include <Base64.h>
#include <EthernetClient.h>
#include <Arduino.h>


WWWsettings::WWWsettings()
{
    
}

void WWWsettings::begin()
{
 
    isDDNSIp = false;
    isEmailIp = false;
    intervalTime = 0;
    previousTime = 0;
    hour = 0;
    minute = 5;
    interval = convertToSeconds(hour,minute,0);

}

void WWWsettings::check()
{
   //**** add non blocking code
    //Serial.println("m ");
    //Serial.println(toEmail);

    if(interval > 0) { // only go through if interval is set
      
        long currentTime = convertToSeconds(::hour(),::minute(),::second());
        //check for interval , every second
     
        
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
 
}

boolean WWWsettings::email(char *in_subject, char *in_message) {
    Serial.println("2noodles mail start");
    EthernetClient client;
    
    if (client.connect("www.2noodles.com", 80)) {
        
        Serial.println("2noodles server connected");
        // Make a HTTP request:
        char emailString[200];
        char key[] = "greencontroller";
        sprintf(emailString, "to=%s&subject=%s&txt=%s&key=%s&", toEmail, in_subject, in_message, key);
        Serial.println(emailString);
        client.print(F("POST /arduino_script/arduino_mail.php"));
        
        client.println(F(" HTTP/1.1"));
        client.println(F("Host: www.2noodles.com"));
        client.println(F("Content-Type: application/x-www-form-urlencoded"));
        client.print(F("Content-Length: "));
        client.println(strlen(emailString));
        client.println(F("Connection: close"));
        client.println();
        client.println(emailString);
        //waitForResponse(client);
        client.stop();
        Serial.println("disconnect");
        return true;
        
    }
    else {
        // didn't get a connection to the server:
        Serial.println("connection failed");
        client.stop();
        return false;
    }

}

void WWWsettings::setNetworkIp(char *in_ip) {
    strcpy(networkIp, in_ip);
}

char * WWWsettings::getNetworkIp(char *in_holder) {
    
    EthernetClient client;
    Serial.println("getting network ip");
    if (client.connect(networkAddress, 80))
    {
        Serial.println("ip connected");
        client.println(F("GET / HTTP/1.0"));
        client.print(F("HOST: "));
        client.println(networkHost);
        client.println();
    } else {
        Serial.println(F("connection failed"));
        client.stop();
        return 0;
        //retrun '\0' works too.
    }
    
    waitTime = millis();
    while (!client.available())
    {
        if((millis() - waitTime) > 10000) {
            client.stop();
            Serial.println(F("\r\nIp Timeout"));
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
    
    EthernetClient client;
    
    
    char userpass[100]={'\0'};
    char output[200] = {'\0'};
    
    sprintf(userpass, "%s:%s", user, password);
    //Serial.println(userpass);
    base64_encode(output,userpass,strlen(userpass));
    //Serial.println(output);
    if (client.connect(ddnsHost, 80))
    {
        Serial.println(F("ddns connected"));
        client.print(F("GET /nic/update?hostname="));
        client.print(ddnsHostName);
        client.println(F(" HTTP/1.0"));
        client.print(F("Host: "));
        client.println(ddnsHost);
        client.print(F("Authorization: Basic "));
        client.println(output);
        client.println(F("User-Agent: Arduino Green Controller/1.0 minh@2noodles.com"));
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
            Serial.println(F("\r\nTimeout"));
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

/*
byte WWWsettings::emailSMTP(char *in_subject, char *in_message)
{
    EthernetClient client;
    //connect to SMTP email server.
    Serial.print(F("connecting SMTP... "));
    Serial.print(smtp);
    Serial.print(" ");
    Serial.println(smtpPort);

    // if you get a connection, report back via serial:
    if (client.connect(smtp, smtpPort))
    {
        Serial.println(F("connected"));
        if(!emailResponse(client)) return 0;
        
        client.print(F("HELO ")); // say hello
        client.println(fromEmail);
        if(!emailResponse(client)) return 0;
        
        client.print(F("MAIL FROM:<")); // identify sender
        client.print(fromEmail);
        client.println(F(">"));
        if(!emailResponse(client)) return 0;
        
        client.print(F("RCPT TO: <")); // identify recipient
        client.print(toEmail);
        client.println(F(">"));
        if(!emailResponse(client)) return 0;
        
        client.println(F("DATA"));
        if(!emailResponse(client)) return 0;
        
        // start of email
        client.print(F("To: "));
        client.println(toEmail);
        client.print(F("From: "));
        client.println(fromEmail);
        client.print(F("Subject: "));
        client.println(in_subject);
        client.print(in_message);
        client.println();
        client.println(F(".")); // end of email
        if(!emailResponse(client)) return 0;
        
        client.println(F("QUIT")); // terminate connection
        Serial.println(F("Email Complete"));
        
        client.stop();
        return 1;
        
    }//endif connect
    if (!client.connected())
    {
        client.stop();
         Serial.println("Disconnected!!!!!!");
        return 0;
       
    }//endif
    
    
}
 */


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


char * WWWsettings::getToEmail() {
    return toEmail;
}

void WWWsettings::setToEmail(char *in_email) {
    strcpy(toEmail, in_email);
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
byte WWWsettings::emailResponse(EthernetClient client)
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


boolean WWWsettings::waitForResponse(EthernetClient client)
{
    waitTime = millis();
    while (!client.available())
    {
        
        if((millis() - waitTime) > 10000) {
            client.stop();
            Serial.println(F("\r\nTimeout"));
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





