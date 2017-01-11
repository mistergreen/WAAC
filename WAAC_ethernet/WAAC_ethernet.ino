#define UNSET -1
#define bufferMax 628
#define queryMax 350
#define W5200_CS  10
#define SDCARD_CS 4
#define DEBUG


#include <SD.h>
#include <SPI.h>

#include <Ethernet.h>
#include <EthernetUdp.h>

// memory check
#include <malloc.h>
//#include <stdlib.h>
#include <stdio.h>
//
#include <utility/w5100.h>
#include <utility/socket.h>

#include <Wire.h>
#include <WebParser.h>
//#include <MemoryFree.h>
#include <Time.h>
//#include <DS1307RTC.h>
//#include "RTClib.h"
#include <dtostrf.h>

//devices - add any necessary libs for devices here!
//for generic device using digial pins or analog
#include <Sensor.h>
#include <Device.h>

//for relays specifically
#include <Relay.h> 

//don't forget to include dependent library into the ino file
#include <OneWire.h>
#include <OneWireSensor.h>
#include <PWM4.h>
#include <Video.h>
#include <Adafruit_PWMServoDriver.h>
#include <AdaFruitPWM8.h>
#include <Alert.h>
#include <Shunt.h>
#include <Analog.h>
#include <Base64.h>
#include <WWWsettings.h>
#include <DeviceDelegate.h>

// pin 10-12 & 4 are off limits SPI & SD card

/*notes
1. i2c devices requires to poke different bytes sequences like a sensor, you'll have to write your own library
2. use Time.h to sync with RTC.. Time.h will sync automatically when set param
3. device object will get time from Time.h instead of RTC.
4. create session for login
5. server will not render html page if enter through url field. If doesn't understand what to render like index.htm

*/
/************* systems **************/
// DS1807 address is 0x68
// CO2_I2C_ADDRESS 0x7F - 'any sensor' default is 0x68 (same as the RTC)
// memory check for 'Due'
extern char _end;
extern "C" char *sbrk(int i);
char *ramstart=(char *)0x20070000;
char *ramend=(char *)0x20088000;


/************* time **************/

//tmElements_t tm;

char *dayName[8] = {"", "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

char *monthName[13] = {"", "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

//RTC_DS1307 rtc;

/************* Device menu **************/
// { device type (must be unique & same as classType), description, html form to configure it }
char *deviceMenu[][3] = {
                        {"Analog","Analog input with Shunt", "analog.htm"},
                        {"AdaFruitPWM8","AdaFruit PWM 12-bit, 8 channel", "adapwm8.htm"},
                        {"Alert","Email Alerts", "alert.htm"},
                        {"OneWireSensor", "OneWire Dallas/Maxim", "onewire.htm"},
                        {"PWM4","Arduino PWM, 4 channels", "pwm4.htm"},
                        {"Relay","Digital out", "relay.htm"},
                        {"Video", "Yout-tube Stream", "video.htm"},
                        {NULL}
                       };

char *deviceStyle[][2] = {
                        {"none.jpg", "FFFFFF"},
                        {"alert.jpg", "FCDF31"},
                        {"fan.jpg", "4B8BE3"},
                        {"light.jpg", "E5B829"},
                        {"motor.jpg", "4B8BE3"},
                        {"relay.jpg", "E55328"},
                        {"rgb.jpg", "E5B829"},
                        {"sensor.jpg", "70B829"},
                        {"thermo.jpg", "70B829"},
                        {"video.jpg", "000000"},
                        {NULL}
                      };
/************* network & server configuration ***************/

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192,168,1,177);
//setting subnet & gateway somehow interfers with calling external php script
//IPAddress gateway(192,168,1,1);	
//IPAddress subnet(255, 255, 255, 0);

 
// Initialize the Ethernet server library
// with the IP address and port you want to use 
// (port 80 is default for HTTP):
EthernetServer server(8080);

int bufferSize;
char queryBuffer[bufferMax];
char param_value[queryMax];
int timeArray[6];
WebParser webParser;

File webFile;
WWWsettings wwws;

/************* website login **************/
char username[] = "admin";
char loginpass[] = "waac";
unsigned long arduinoSession = 1;

  
/****************************************** sketch Logic **********************************************************/

void setup() 
{
 
  // start serial port:
  Serial.begin(115200);
  //SerialUSB.begin(115200);  // Due nativeUSB port, no reset of board on open serial port
  
  
  #ifdef AVR
    Wire.begin();
  #else
    Wire1.begin(); // for Arduino Due
  #endif

  
  //sync RTC time
 
   //rtc.begin();

  //if (! rtc.isrunning()) {
    //Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
   // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
 // }
   
  //setSyncProvider(syncProvider);   // the function to get/sync the time from the RTC - occurs every 5 mins?, you can change the interval 
 
  

  //********************** manually add I2C devices for the app **********************
  //                                     name,  i2c address, low target, high target
  //CO2sensor *co2 = new CO2sensor("CO2 sensor",0x7F, 0, 43);
 // deviceDelegate.addDevice(co2);
  //deviceDelegate.currentDevice()->setStyle(deviceStyle[3][0],deviceStyle[3][1]);
  /*
  OneWireSensor *one = new OneWireSensor("Water temp", 2, 0, 100);
  deviceDelegate.addDevice(one);
  deviceDelegate.currentDevice()->setStyle(deviceStyle[7][0],deviceStyle[7][1]);
  static_cast<OneWireSensor*>(deviceDelegate.currentDevice())->setLowerTarget(70.0);
  static_cast<OneWireSensor*>(deviceDelegate.currentDevice())->setHigherTarget(81.0);
  */
  
  //temp, no interface
// relay time event trigger a crash!?  
  /*
  Relay *tempObj = new Relay("LED", 22, 0);
  deviceDelegate.addDevice( tempObj );
  deviceDelegate.currentDevice()->setStyle(deviceStyle[3][0],deviceStyle[3][1]);
  char temp[] = "10:00:00,1:30:00,1111111,12:30:00,5:00:00,1010101";
  deviceDelegate.currentDevice()->setEvent(temp);
  */
 //temp, no interface
 /*
  deviceDelegate.addDevice( new PWM4("PWM Lights",0) );
  deviceDelegate.currentDevice()->setStyle(deviceStyle[3][0],deviceStyle[3][1]);
  static_cast<PWM4*>(deviceDelegate.currentDevice())->setPins(8,7,6,5);
  char temp2[] = "1111111,8:00:00,0:30:00,100:10:255:50,9:30:00,0:00:10,255:50:30:255";
  deviceDelegate.currentDevice()->setEvent(temp2);
  
  deviceDelegate.addDevice( new AdaFruitPWM("Ada PWM",0,0) );
  deviceDelegate.currentDevice()->setStyle(deviceStyle[3][0],deviceStyle[3][1]);
  char temp3[] = "1111111,8:00:00,0:30:00,100,9:30:00,0:00:10,4055";
  deviceDelegate.currentDevice()->setEvent(temp3);
  */
 
  /*
  deviceDelegate.addDevice( new AdaFruitPWM4("AdaPWM Lights",0) );
  deviceDelegate.currentDevice()->setStyle(deviceStyle[3][0],deviceStyle[3][1]);
  static_cast<AdaFruitPWM4*>(deviceDelegate.currentDevice())->setPins(0,1,2,3);
  char temp2[] = "1111111,8:00:00,0:30:00,100:10:255:50,9:30:00,0:00:10,255:50:30:255";
  deviceDelegate.currentDevice()->setEvent(temp2);
 */
  /*
   deviceDelegate.addDevice( new AdaFruitPWM8("Pendant Light",0) );
  deviceDelegate.currentDevice()->setStyle(deviceStyle[3][0],deviceStyle[3][1]);
  static_cast<AdaFruitPWM8*>(deviceDelegate.currentDevice())->setPins(0,1,2,3,4,5,6,7);
  char temp2[] = "1111111,8:00:00,0:30:00,100:10:255:50:100:10:255:50,9:30:00,0:00:10,255:50:30:255:100:10:255:50";
  deviceDelegate.currentDevice()->setEvent(temp2);
  */
  
  /*
  deviceDelegate.addDevice( new Video("Aquarium Stream", "owTmdfiTjIM") );
  deviceDelegate.currentDevice()->setStyle(deviceStyle[8][0],deviceStyle[8][1]);
  */

  /*
  deviceDelegate.addDevice( new Alert("Temperature Alert", "5137036979@vtext.com", 0) );
  deviceDelegate.currentDevice()->setStyle(deviceStyle[1][0],deviceStyle[1][1]);
  */
  
  /*
  deviceDelegate.addDevice( new Analog("Temperature Analog", A0) );
  deviceDelegate.currentDevice()->setStyle(deviceStyle[1][0],deviceStyle[1][1]);
  */
  
  //************ initialize SD card & ethernet*******************
  //Ethernet.begin(mac, ip, INADDR_NONE, gateway, subnet);
  Ethernet.begin(mac, ip);
  //set up upd first so it gets sockets #0
  wwws.begin();
  
  // set up server for the rest of the sockets
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
  
  //pinMode(W5200_CS, OUTPUT);
  //disconnect the W5200
  //digitalWrite(W5200_CS,HIGH);
  
  //pinMode(SDCARD_CS,OUTPUT);
  
  //Serial.println("Initializing SD card...");
  if (!SD.begin(SDCARD_CS)) {
        Serial.println("ERROR - SD card initialization failed!");
        return;    // init failed
  }
  //Serial.println("SUCCESS - SD card initialized.");
    // check for index.htm file
    //it's picky with file extension, only 3 letters
  
  if (!SD.exists("index.htm")) {
        Serial.println("ERROR - Can't find index.htm file!");
        return;  // can't find index file
  }
 
  Serial.println("SUCCESS - Found index.htm file.");
    
  //digitalWrite(SDCARD_CS, HIGH);
  //let the ethernet card get ready before peforming tasks

  //wwws.setToEmail("minh@2noodles.com");
  //wwws.setEvent(0,5);
  //wwws.setNetworkAddress("api.ipify.org");
  //wwws.setNetworkHost("api.ipify.org");
  //wwws.setEmailIpState(true);
  //setDdnsHost("dynupdate.no-ip.com");
  //setDdnsHostName("greencontroller.ddns.net");
  
  //setUser("mistergreen15");
  //setPassword("mynoip123");

  delay(10000);
  
  //sync time to NTP
  wwws.syncNTP();
  
  Serial.print("memory ");
  Serial.println(freeMemory());
  
  
}

/************************************************************ loop *********************************/
// connectLoop controls the hardware fail timeout
int connectLoop = 0;
  
void loop() 
{
  //Serial.println("x");
  
  //loop through devices
  deviceDelegate.loopDevices();
   //Serial.println("y");
  //www settings
  wwws.check();
  //Serial.println("z");
  // listen for incoming clients
  EthernetClient client = server.available();
 
  if (client) {
    
     boolean currentLineIsBlank = true;
     bufferSize = 0;
     
        while (client.connected()) {
            if (client.available()) {   // client data available to read
                char c = client.read(); // read 1 byte (character) from client
                //Serial.println(c);
                if (bufferSize < bufferMax) queryBuffer[bufferSize++] = c;
                 if (c == '\n' && currentLineIsBlank) {
                   parseReceivedRequest(client);
                   bufferSize = 0;
                   webParser.clearBuffer(queryBuffer, bufferMax);
                   break;
                     
                 }
                 if (c == '\n') {
                    // last character on line of received text
                    // starting new line with next character read
                    currentLineIsBlank = true;
                } 
                else if (c != '\r') {
                    // a text character was received from client
                    currentLineIsBlank = false;
                }
                connectLoop = 0;
                
            }//end if available
           /*
            connectLoop++;
            // if more than 10000 milliseconds since the last packet
            if(connectLoop > 10000)
            {
              // then close the connection from this end.
              Serial.println();
              Serial.println(F("Timeout"));
              client.stop();
            }
          */
         
    
        }//end while connected
    
    
      // give the web browser time to receive the data
      delay(1);
    // close the connection:
    client.stop();
    //disconnect the W5200
    //digitalWrite(W5200_CS,HIGH);
    
    //Serial.println("client disonnected");
   //
    
  }//end client
  
  //*******socket

  if(Serial.available()) {
    if(Serial.read() == 'r') ShowSockStatus();    
  }
  

  //checkSockStatus();

  
}

/*********************************/
boolean loggedIn() 
{
   webParser.clearBuffer(param_value,queryMax);
   //going to need a parse cookie function
   webParser.parseQuery(queryBuffer, "ARDUINOSESSIONID", param_value);
   //Serial.println("vvvvvlogin");
   //Serial.println(arduinoSession);
   //Serial.println(param_value);
   //Serial.println(atol(param_value));
   if(arduinoSession == atol(param_value)) {
      return true;
   } else {
      return false; 
   }
}

void renderHtmlPage(char *page, EthernetClient client) 
{

        //disconnect the W5200
        //digitalWrite(W5200_CS,HIGH);      
        byte tBuf[64];
        int clientCount = 0;
        unsigned long lastPosition = 0;
        
        File myFile = SD.open(page);        // open web page file
        if (myFile) {
             // send a standard http response header
             client.println(F("HTTP/1.1 200 OK"));
          
             if(webParser.contains(page, ".jpg")){
               client.println(F("Content-Type: image/jpeg"));
               client.println(F("Cache-Control: max-age=36000, public"));  
             } else if(webParser.contains(page, ".gif")){
               client.println(F("Content-Type: image/gif"));
               client.println(F("Cache-Control: max-age=36000, public"));
             } else if(webParser.contains(page, ".png")){
               client.println(F("Content-Type: image/png"));
               client.println(F("Cache-Control: max-age=36000, public"));
             } else if(webParser.contains(page, ".htm")){
               client.println(F("Content-Type: text/html"));
             } else if(webParser.contains(page, ".js")){
               client.println(F("Content-Type: application/javascript"));
             } else if(webParser.contains(page, ".css")){
               client.println(F("Content-Type: text/css"));
             }

             client.println();   
        
           while(myFile.available())
            {
               myFile.read(tBuf,64); // or myFile.read(&tBuf,64)
               client.write(tBuf, myFile.position() - lastPosition);
               lastPosition = myFile.position();
    
             }
              
            myFile.close();
             // disconnect the SD card
             //digitalWrite(SDCARD_CS, HIGH);
        } else {
          Serial.println("file not found"); 
          Serial.println(page);
           client.println(F("HTTP/1.1 404 Not Found"));
           client.println(F("Content-Type: text/html"));
           client.println();
           client.println(F("<h2>File Not Found!</h2>"));
        }
        
       
        
}

void successAjax(EthernetClient client) {
       //ajax requires a response or will throw a connection lost
      client.println(F("HTTP/1.1 200 OK"));
      client.println(F("Content-Type: text/xml"));
      client.println();
      client.println(F("<?xml version = \"1.0\" ?>"));
      client.print(F("<response>success</response>"));
}

void errorAjax(EthernetClient client) {
        client.println(F("HTTP/1.1 200 OK"));
        client.println(F("Content-Type: text/xml"));
        client.println();
        client.println(F("<?xml version = \"1.0\" ?>"));
        client.print(F("<response>error</response>"));
}

void parseReceivedRequest(EthernetClient client) 
{
  //find query vars
  //Serial.println(" ");
  //Serial.println("*************");
  //Serial.println(queryBuffer);
  //Serial.println("*************");
  
  //  GET /index.htm HTTP/1.1
  // GET / HTTP/1.1
  if(webParser.contains(queryBuffer, "GET / HTTP/1.1") || webParser.contains(queryBuffer, ".htm ")) {
    // *********** Render HTML ***************
   // code not to render xml request.
   // GET /index.htm?devicelist=1&nocache=549320.8093103021 HTTP/1.1

    if(loggedIn()) {
       //render html pages
       webParser.clearBuffer(param_value, queryMax);
       webParser.fileUrl(queryBuffer, param_value);
       //Serial.print("xxxx render html");
       //Serial.println(param_value);
       // in case no html is defined
       if(strcmp(param_value, "/") == 0) {
         strcpy(param_value, "index.htm");
         client.println(F("HTTP/1.1 302 Found"));
         client.println(F("Location: /index.htm")); 

       }
       renderHtmlPage(param_value, client);
       
     } else {  
        //loggin form
        Serial.print("not logged in");
        char page[] = "login.htm";
        //set it so it's not the same all the time.
        arduinoSession = millis();
        renderHtmlPage(page, client);
        
    }//login

   } else if(webParser.contains(queryBuffer, ".jpg ") || webParser.contains(queryBuffer, ".gif ") || webParser.contains(queryBuffer, ".png ") 
             || webParser.contains(queryBuffer, ".js ") || webParser.contains(queryBuffer, ".css "))
   {
       //handle jpg images & other files
       //get url 
       //Serial.print("image and such");
       webParser.clearBuffer(param_value, queryMax);
       webParser.fileUrl(queryBuffer, param_value);
       renderHtmlPage(param_value, client);

   } else {
    //send xml data to client
    //POST method's data goes after a new blank line. Causing issue to parse
    //will use ajax instead
    webParser.clearBuffer(param_value, queryMax);
    
    if(webParser.contains(queryBuffer, "login=1")) 
    {
      webParser.parseQuery(queryBuffer, "username", param_value);

      char user[30];
      strcpy(user,param_value);

      webParser.clearBuffer(param_value, queryMax);
      webParser.parseQuery(queryBuffer, "password", param_value);
      char pass[30];
      strcpy(pass,param_value);
      Serial.println("user pass");
       
       // ***************** LOGIN ********************   
      if(webParser.compare(username,user) && webParser.compare(loginpass,pass)) {
          
        //Serial.println("logged inxxxxx ");
          arduinoSession = millis();
          Serial.println(arduinoSession);
          client.println(F("HTTP/1.1 200 OK"));
          client.println(F("Content-Type: text/xml"));
          client.print(F("Set-cookie: ARDUINOSESSIONID="));
          client.print(arduinoSession);
          client.println(F("; HttpOnly"));
          //client.println("Connection: keep-alive");
          client.println();
          client.println(F("<?xml version = \"1.0\" ?>"));
          client.print(F("<response>login</response>"));
        
      } else {
          client.println(F("HTTP/1.1 200 OK"));
          client.println(F("Content-Type: text/xml"));
          //client.println("Connection: keep-alive");
          client.println();
          client.println(F("<?xml version = \"1.0\" ?>"));
          client.print(F("<response>deny</response>"));
      }
        
    } 
    // ***************** list devices in index.htm  ********************   
    else if(webParser.contains(queryBuffer, "devicelist=1")) 
    {
      
      webParser.clearBuffer(param_value, queryMax);
      client.println(F("HTTP/1.1 200 OK"));
      client.println(F("Content-Type: text/xml"));
      //client.println("Connection: keep-alive");
      client.println();
      
      client.println(F("<?xml version = \"1.0\" encoding=\"UTF-8\"?>"));
      client.print(F("<list>"));
      client.print(F("<time><hour>"));
      client.print(hour());
      client.print(F("</hour><minute>"));
      client.print(minute());
      client.print(F("</minute><second>"));
      client.print(second());
      client.print(F("</second><weekday>"));
      client.print(dayName[weekday()]);
      client.print(F("</weekday><month>"));
      client.print(monthName[month()]);
      client.print(F("</month><day>"));
      client.print(day());
      client.print(F("</day><year>"));
      client.print(year());
      client.println(F("</year></time>"));
      client.print(F("<memory>"));
      client.print(freeMemory());
      client.println(F("</memory>"));
      
      for(uint8_t i = 0; i < deviceDelegate.getDeviceCount(); i++) {
        client.print(F("<device><name>"));
        client.print(deviceDelegate.devices[i]->getDeviceName());
        client.print(F("</name><value>"));
        //client.print(F("<![CDATA["));
        client.print(deviceDelegate.devices[i]->read());
        //client.print(F("]]>"));
        client.print(F("</value>"));
          if(!deviceDelegate.devices[i]->isAlert()) {
            client.print(F("<output>"));
            if(deviceDelegate.devices[i]->isI2C()) {
              client.print(F("I2C: "));
            } else {
              client.print(F("PIN: "));
            }
            client.print(deviceDelegate.devices[i]->getPin());
             
          client.print(F("</output>"));
          }
         if(deviceDelegate.devices[i]->isVideo()) {
            client.print(F("<video>"));
            client.print( static_cast<Video*>(deviceDelegate.devices[i])->getCode() );
            client.print(F("</video>"));
          }
          
        client.print(F("<image>"));
        client.print(F("images/"));
        client.print(deviceDelegate.devices[i]->getImageName());
        client.print(F("</image>"));
        client.print(F("<color>"));
        client.print(deviceDelegate.devices[i]->getDeviceColor());
        client.print(F("</color>"));
        client.print(F("<id>"));
        client.print(deviceDelegate.devices[i]->getDeviceId());
        client.print(F("</id><state>")); 
        client.print(deviceDelegate.devices[i]->getDeviceState());
        client.print(F("</state><type>"));
        client.print(deviceDelegate.devices[i]->getClassName());
        client.println(F("</type></device>"));
         
      }// for list
      
      client.println(F("</list>"));
     
    } 
    // ***************** switch digital  ********************
    else if(webParser.contains(queryBuffer, "switch="))
    {
      //will work for all device type with toggleState()
      switchDigital(client);
        
    } 
    
      // ***************** set PWM values  ********************
    else if(webParser.contains(queryBuffer, "setpwms"))
    {

      set4PWM(client);
        
    } 
    
     // ***************** set adafruit PWM value 1 channel ********************
     /*
    else if(webParser.contains(queryBuffer, "setadapwm"))
    {
     
     setAdaPWM(client);
        
    } */
      // ***************** set ada PWM 4 channels  ********************
      /*
    else if(webParser.contains(queryBuffer, "set4adapwms"))
    {
     //gets confused with setadapwm
      //Serial.println(queryBuffer);
      set4AdaPWMs(client);

        
    } */
      // ***************** set ada PWM 8 channels  ********************
    else if(webParser.contains(queryBuffer, "set8adapwms"))
    {
     //gets confused with setadapwm
      //Serial.println(queryBuffer);
      set8AdaPWMs(client);

        
    } 
      // ***************** set  PWM value 1 channel ********************
      /*
    else if(webParser.contains(queryBuffer, "setpwm"))
    {

      
      setPWM(client);
        
    } */
       // ***************** Test Email ********************
    else if(webParser.contains(queryBuffer, "testemail"))
    {
       testEmail(client);
    } 
        // ***************** Test IP ********************
    else if(webParser.contains(queryBuffer, "testip"))
    {
      testIp(client);
        
    } 
          // ***************** Test DDNS ********************
    else if(webParser.contains(queryBuffer, "testddns"))
    {
       testDNS(client);
        
    } 
    // ***************** Test NTP ********************
    else if(webParser.contains(queryBuffer, "testntp"))
    {
       testNTP(client);
        
    } 
    // ***************** SAVE Settings ********************
    else if(webParser.contains(queryBuffer, "savesetting"))
    {
      saveSetting(client);
        
    } 
     // ***************** LOAD Settings ********************
    else if(webParser.contains(queryBuffer, "getsetting"))
    {
     
       getSetting(client);

    } 
    // ***************** logout  ********************
    else if(webParser.contains(queryBuffer, "logout=1"))
    {
          arduinoSession = 1;
          client.println(F("HTTP/1.1 200 OK"));
          client.println(F("Content-Type: text/xml"));
          client.println();
          client.println(F("<?xml version = \"1.0\" ?>"));
          client.print(F("<response>logout</response>"));
    
    } 
    // ***************** menu for select.htm  ********************
    else if(webParser.contains(queryBuffer, "devicemenu"))
    {
      
      //webParser.clearBuffer(param_value,30); 
       // device id will be kept in localStorage on the browser
          client.println(F("HTTP/1.1 200 OK"));
          client.println(F("Content-Type: text/xml"));
          client.println();
          client.println(F("<?xml version = \"1.0\" ?>"));
          client.print(F("<menu>"));
          uint8_t i = 0;
          while(deviceMenu[i][0] != NULL) {
            
            Serial.println(deviceMenu[i][0]);
             client.print(F("<item>"));
                client.print(F("<name>"));
                client.print(deviceMenu[i][0]);
                client.print(F("</name>"));
                client.print(F("<description>"));
                client.print(deviceMenu[i][1]);
                client.print(F("</description>"));
                 client.print(F("<page>"));
                client.print(deviceMenu[i][2]);
                client.print(F("</page>"));
             client.print(F("</item>"));
            i++;
          }
          client.print(F("</menu>"));
         
    } 
    // ***************** get .htm for what device  ********************
    else if(webParser.contains(queryBuffer, "gettemplate"))
    {
          //
          //Serial.println(queryBuffer);
          webParser.clearBuffer(param_value, queryMax);
          webParser.parseQuery(queryBuffer, "gettemplate", param_value);
      //Serial.println(param_value);
          //Serial.println("getting template");
          Device *device = deviceDelegate.findDevice(atoi(param_value));

          webParser.clearBuffer(param_value,30);
          strcpy(param_value, device->getClassName());
          
          client.println(F("HTTP/1.1 200 OK"));
          client.println(F("Content-Type: text/xml"));
 
          client.println();
          client.println(F("<?xml version = \"1.0\" ?>"));
          client.print(F("<template>"));
          uint8_t i = 0;
          while(deviceMenu[i][0] != NULL) {
            if(webParser.compare(param_value, deviceMenu[i][0])) {
               client.print(F("<page>"));
               client.print(deviceMenu[i][2]);
               client.print(F("</page>"));
               client.print(F("<type>"));
               client.print(deviceMenu[i][0]); // param_value
               client.print(F("</type>"));
               
            }
            i++;
            
          }
          client.print(F("</template>"));
   
    } 
    // ***************** populate device data for .htm  ********************
    else if(webParser.contains(queryBuffer, "deviceid"))
    {
          //
          webParser.clearBuffer(param_value, queryMax);
          webParser.parseQuery(queryBuffer, "deviceid", param_value);
     
          //Serial.println(param_value);
          
          client.println(F("HTTP/1.1 200 OK"));
          client.println(F("Content-Type: text/xml"));
          client.println();
          client.println(F("<?xml version = \"1.0\" encoding=\"UTF-8\"?>"));
          client.print(F("<deviceItem>"));
          
          //get device specific info
          if(atoi(param_value) != 0) {
            Device *device = deviceDelegate.findDevice(atoi(param_value));
            device->setSuspendTime(true);
            client.print(F("<deviceName>"));
            client.print(device->getDeviceName());
            client.print(F("</deviceName>"));
            client.print(F("<deviceImage>"));
            client.print(device->getImageName());
            client.print(F("</deviceImage>"));
            client.print(F("<deviceColor>"));
            client.print(device->getDeviceColor());
            client.print(F("</deviceColor>"));
            client.print(F("<state>"));
            client.print(device->getDeviceState());
            client.print(F("</state>"));
            
            webParser.clearBuffer(param_value, queryMax);
            webParser.parseQuery(queryBuffer, "devicetype", param_value);
            //Serial.println("devicetype ");
            //Serial.println(param_value);
            if(webParser.compare(param_value, "Relay")) {
                
              relayAjaxOutput(client, device);
              
            } else if(webParser.compare(param_value, "OneWireSensor")) 
            {
               
              oneWireAjaxOutput(client, device);
               
            } 
            else if(webParser.compare(param_value, "Analog")) 
            {
               
              analogAjaxOutput(client, device);
               
            } 
            else if(webParser.compare(param_value, "PWM4")) 
            {
               pwm4AjaxOutput(client, device);
            } /*
            else if(webParser.compare(param_value, "AdaFruitPWM4")) 
            {
               adaFruit4PWMAjaxOutput(client, device);
            } */
            else if(webParser.compare(param_value, "AdaFruitPWM8")) 
            {
               adaFruit8PWMAjaxOutput(client, device);
            } 
            else if(webParser.compare(param_value, "Video")) {
               videoAjaxOutput(client, device);
                
            }
            else if(webParser.compare(param_value, "Alert")) 
            {
               alertAjaxOutput(client, device);
                
            }
            
          }
          
          //menus
          uint8_t i = 0;
          while(deviceStyle[i][0] != NULL) {
            client.print(F("<item>"));
               client.print(F("<image>"));
               client.print(deviceStyle[i][0]);
               client.print(F("</image>"));
               client.print(F("<color>"));
               client.print(deviceStyle[i][1]);
               client.print(F("</color>"));
            client.print(F("</item>"));
            i++;
          }
          
           
              //get devices name & id for dependent list. Print for everybody. They can use with it what they will.
              // saves adding on to the condition
             for(i = 0; i < deviceDelegate.getDeviceCount(); i++) {
                client.print(F("<device>"));
                client.print(F("<name>"));
                client.print(deviceDelegate.devices[i]->getDeviceName());
                client.print(F("</name>"));
                client.print(F("<id>"));
                client.print(deviceDelegate.devices[i]->getDeviceId());
                client.print(F("</id>"));
                client.print(F("</device>"));
             }
         
            
          client.print(F("</deviceItem>"));
       
         
    } 
    // ***************** save or create device object  ********************
    else if(webParser.contains(queryBuffer, "savedevice"))
    {
     
      //Serial.println(queryBuffer);
      
      webParser.clearBuffer(param_value, queryMax);
      webParser.parseQuery(queryBuffer, "savedevice", param_value);
        
        if(atoi(param_value) > 0) {
          // Serial.print("xxxxxxx save device xxxxxxxx");
          //alter existing device
          Device *device = deviceDelegate.findDevice(atoi(param_value));
          device->setSuspendTime(false);
          
          webParser.clearBuffer(param_value, queryMax);
          webParser.parseQuery(queryBuffer, "name", param_value);
          device->setDeviceName(param_value);
          
          webParser.clearBuffer(param_value, queryMax);
          webParser.parseQuery(queryBuffer, "pin", param_value);
          device->setPin(atoi(param_value));
          
          webParser.clearBuffer(param_value, queryMax);
          webParser.parseQuery(queryBuffer, "image", param_value);
          device->setImageName(param_value);
          
          char color[7];
          findColor(param_value, color);
          device->setDeviceColor(color);

          webParser.clearBuffer(param_value, queryMax);
          webParser.parseQuery(queryBuffer, "devicetype", param_value);
          
          if(webParser.compare(param_value, "Relay"))
          {
               saveRelay(device);
          
          } 
          else if(webParser.compare(param_value, "OneWireSensor")) 
          {
           
               saveOneWire(device);
           
          } 
           else if(webParser.compare(param_value, "Analog")) 
          {
               saveAnalog(device);
           
          } 
          else if(webParser.compare(param_value, "PWM4")) 
          {
               savePWM4(device);
          }  
          /*
          else if(webParser.compare(param_value, "AdaFruitPWM4")) 
          {
            
              saveAdaFruit4PWM(device);
          }  */
          else if(webParser.compare(param_value, "AdaFruitPWM8")) 
          {
            
              saveAdaFruit8PWM(device);
          }  
          else if(webParser.compare(param_value, "Video")) 
          {
              saveAVideo(device);
           
            
          }
           else if(webParser.compare(param_value, "Alert")) 
          {
             saveAlert(device);
            
          }
          
          
        } else {
          //create new object 
          //Serial.print("xxxxxxx created device xxxxxxxx");
          webParser.clearBuffer(param_value, queryMax);
          webParser.parseQuery(queryBuffer, "devicetype", param_value);
          //******************* RELAY ***********************
          if(webParser.compare(param_value, "Relay")) {
              createRelay();
          } 
          else if(webParser.compare(param_value, "AdaFruitPWM8")) {
              createAdaFruit8PWM();
          }
          else if(webParser.compare(param_value, "Alert")) {
              createAlert();
          }
          else if(webParser.compare(param_value, "Analog")) {
              createAnalog();
          }
          else if(webParser.compare(param_value, "OneWireSensor")) {
              createOneWire();
          }
          else if(webParser.compare(param_value, "PWM4")) {
              createPWM4();
          }
          else if(webParser.compare(param_value, "Video")) {
              createVideo();
          }

        }
        //response back to client on success of saving or creating
        successAjax(client);
    }
    // ***************** delete device object  ********************
    else if(webParser.contains(queryBuffer, "deletedevice"))
    {
        
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "deletedevice", param_value);
        deviceDelegate.removeDevice(atoi(param_value));
        
        successAjax(client);
    }
    // ***************** cancel device changes  ********************
    else if(webParser.contains(queryBuffer, "canceldevice"))
    {
        
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "canceldevice", param_value);
        
        Device *device = deviceDelegate.findDevice(atoi(param_value));
        device->setSuspendTime(false);
          
        successAjax(client);
        
    }// end else if
    
    
  }//end main else
}// end function

//*********
void findColor(char *image, char *ar) {
  uint8_t i=0;
   while(deviceStyle[i][0] != NULL) {
      if(webParser.compare(image, deviceStyle[i][0])) {
        strncpy(ar, deviceStyle[i][1], 7);
        
        break;
      }
      i++;       
    }
              
}

//****************** RTC stuff ************************************
/*
time_t syncProvider()
{
  //this does the same thing as RTC_DS1307::get()
  return rtc.now().unixtime();
}


bool getTime(const char *str)
{
  int Hour, Min, Sec;

  if (sscanf(str, "%d:%d:%d", &Hour, &Min, &Sec) != 3) return false;
  tm.Hour = Hour;
  tm.Minute = Min;
  tm.Second = Sec;
  return true;
}

bool getDate(const char *str)
{
  char Month[12];
  int Day, Year;
  uint8_t monthIndex;

  if (sscanf(str, "%s %d %d", Month, &Day, &Year) != 3) return false;
  for (monthIndex = 0; monthIndex < 13; monthIndex++) {
    if (strcmp(Month, monthName[monthIndex]) == 0) break;
  }
  if (monthIndex >= 13) return false;
  tm.Day = Day;
  tm.Month = monthIndex;
  tm.Year = CalendarYrToTm(Year);
  return true;
}
*/

int freeMemory() {
  char *heapend=sbrk(0);
  register char * stack_ptr asm ("sp");
  struct mallinfo mi=mallinfo();
  //printf("\nDynamic ram used: %d\n",mi.uordblks);
  //printf("Program static ram used %d\n",&_end - ramstart); 
  //printf("Stack ram used %d\n\n",ramend - stack_ptr); 
  int memory = stack_ptr - heapend + mi.fordblks;
  //printf("My guess at free mem: %d\n",memory);
  return memory;
  
}


//********************** socket ********************
byte socketStat[MAX_SOCK_NUM];
unsigned long connectTime[MAX_SOCK_NUM];

void ShowSockStatus()
{
  for (int i = 0; i < MAX_SOCK_NUM; i++) {
    Serial.print(F("Socket#"));
    Serial.print(i);
    uint8_t s = W5100.readSnSR(i);
    socketStat[i] = s;
    Serial.print(F(":0x"));
    Serial.print(s,16);
    Serial.print(F(" "));
    Serial.print(W5100.readSnPORT(i));
    Serial.print(F(" D:"));
    uint8_t dip[4];
    W5100.readSnDIPR(i, dip);
    for (int j=0; j<4; j++) {
      Serial.print(dip[j],10);
      if (j<3) Serial.print(".");
    }
    Serial.print(F("("));
    Serial.print(W5100.readSnDPORT(i));
    Serial.println(F(")"));
  }
}

void checkSockStatus()
{
  unsigned long thisTime = millis();

  for (int i = 0; i < MAX_SOCK_NUM; i++) {
    uint8_t s = W5100.readSnSR(i);

    if((s == 0x17) || (s == 0x1C)) {
        if(thisTime - connectTime[i] > 30000UL) {
          Serial.print(F("\r\nSocket frozen: "));
          Serial.println(i);
          close(i);
        }
    }
    else connectTime[i] = thisTime;

    socketStat[i] = W5100.readSnSR(i);
  }
}



