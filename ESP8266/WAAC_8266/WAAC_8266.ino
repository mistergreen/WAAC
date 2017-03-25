// © 2noodles llc
// minh@2noodles.com

#define UNSET -1
#define bufferMax 628
#define queryMax 350
#define SDCARD_CS D8

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#include <SPI.h>
#include <SD.h>

//#include <Ethernet.h>
#include <Wire.h>
#include <WebParser.h>
#include <TimeLib.h>
//#include <dtostrf.h>
#include <rBase64.h>

//devices - add any necessary libs for devices here!
//for generic device using digial pins or analog
#include <Sensor.h>
#include <Device.h>

//for relays specifically
#include <Relay.h> 
#include <ArduinoRelay.h> 
//don't forget to include dependent library into the ino file
#include <OneWire.h>
#include <OneWireSensor.h>

#include <PWM4.h>
#include <Video.h>
#include <Adafruit_PWMServoDriver.h>
#include <AdaFruitPWM8.h>
#include <Alert.h>
#include <rBase64.h>
#include <Hash.h>
#include <WWWsettings.h>
#include <DeviceDelegate.h>
#include <String.h>

// pin 10-12 & 4 are off limits SPI & SD card


/************* time **************/

//tmElements_t tm;

const char *dayName[8] = {"", "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

const char *monthName[13] = {"", "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};


/************* Device menu **************/
// { device type (must be unique & same as classType), description, html form to configure it }
const char *deviceMenu[8][3] = {
                        {"AdaFruitPWM8","AdaFruit PWM 12-bit, 8 channel", "adapwm8.htm"},
                        {"Alert","Email Alerts", "alert.htm"},
                        {"OneWireSensor", "OneWire Dallas/Maxim", "onewire.htm"},
                        {"PWM4","ESP PWM, 4 channels", "pwm4.htm"},
                        {"Relay","Native Digital out", "relay.htm"},
                        {"ArduinoRelay","Arduino Digital out", "relay.htm"},
                        {"Video", "Yout-tube Stream", "video.htm"},
                        {NULL}
                       };

const char *deviceStyle[11][2] = {
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
IPAddress gateway(192,168,1,1);	
IPAddress subnet(255, 255, 255, 0);

// Initialize the Ethernet server library
// with the IP address and port you want to use 
// (port 80 is default for HTTP):
WiFiServer server(8080);

int status = WL_IDLE_STATUS;
int bufferSize;
char queryBuffer[bufferMax];
char param_value[queryMax];
int timeArray[6];
WebParser webParser;

File webFile;
WWWsettings wwws;

/************* website login **************/
// wifi
char ssid[] = "WATERFALL"; //  your network SSID (name)
char password[] = "quynhnhi";    // your network password (use for WPA, or use as key for WEP)

// site login
char username[] = "admin";
char loginpass[] = "waac";


unsigned long arduinoSession = 1;

  
/****************************************** sketch Logic **********************************************************/

void setup() 
{
 
  // start serial port:
  Serial.begin(115200);
  //SerialUSB.begin(115200);  // Due nativeUSB port, no reset of board on open serial port

  //Wire.begin();

  //************ wifi *******************
  
  WiFi.config(ip, gateway, subnet, gateway);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("Connected to wifi");
  printWifiStatus();

  // set up server for the rest of the sockets
  server.begin();
  
  //set up upd first so it gets sockets #0
  wwws.begin();
  

   //************ initialize SD card *******************
  
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

  delay(10000);
  
  //sync time to NTP
  wwws.syncNTP();
  
  Serial.print("memory ");
  Serial.println(freeMemory());

  pins();
  
}

void pins() {
  //Serial.print("A0: ");
  //Serial.println(atoi("A0"));
  /*
A0: 17
D0: 16
D1: 5
D2: 4
D3: 0
D4: 2
D5: 14
D6: 12
D7: 13
D8: 15

SDA: 4
SCL: 5

Who is takingup d4, d3?
d3, d4 looks to be in use permenantly
   * 
   */
  
  Serial.print("A0: ");
  Serial.println(A0);
  Serial.print("D0: ");
  Serial.println(D0);
  Serial.print("D1: ");
  Serial.println(D1);
  Serial.print("D2: ");
  Serial.println(D2);
  Serial.print("D3: ");
  Serial.println(D3);
    Serial.print("D4: ");
  Serial.println(D4);
    Serial.print("D5: ");
  Serial.println(D5);
    Serial.print("D6: ");
  Serial.println(D6);
  Serial.print("D7: ");
  Serial.println(D7);
  Serial.print("D8: ");
  Serial.println(D8);

    Serial.print("SDA: ");
  Serial.println(SDA);

      Serial.print("SCL: ");
  Serial.println(SCL);

  //prepare pins
  pinMode(D0, OUTPUT);
  digitalWrite(D0, 0);
  pinMode(D1, OUTPUT);
  digitalWrite(D1, 0);
  pinMode(D2, OUTPUT);
  digitalWrite(D2, 0);
  pinMode(D3, OUTPUT);
  digitalWrite(D3, 0);
  pinMode(D4, OUTPUT);
  digitalWrite(D4, 0);

}

int convertPin(char *pin){
  Serial.print("pin: ");
  Serial.println(pin);
  if(webParser.contains(pin, "A0")) {
    return A0;
  } else if(webParser.contains(pin, "D1")) {
    return D1;
  } else if(webParser.contains(pin, "D2")) {
    return D2;
  } else if(webParser.contains(pin, "D3")) {
    return D3;
  } else if(webParser.contains(pin, "D4")) {
    return D4;
  } else if(webParser.contains(pin, "D5")) {
    return D5;
  } else if(webParser.contains(pin, "D6")) {
    return D6;
  } else if(webParser.contains(pin, "D7")) {
    return D7;
  } else if(webParser.contains(pin, "D8")) {
    return D8;
  }
   
    Serial.print("can't find pin: ");
    Serial.println(atoi(pin));
    return atoi(pin);
    
  

}


/************************************************************ loop *********************************/
  
void loop() 
{
 
  //loop through devices
  deviceDelegate.loopDevices();
   //Serial.println("y");
  //www settings
  wwws.check();
  //Serial.println("z");
  // listen for incoming clients
  WiFiClient client = server.available();
 
  if (client) {
    
     boolean currentLineIsBlank = true;
     bufferSize = 0;
     
        while (client.connected()) {
            if (client.available()) {   // client data available to read
                char c = client.read(); // read 1 byte (character) from client
               
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
               
                
            }//end if available
          
        }//end while connected
    
    
      // give the web browser time to receive the data
      delay(1);
    // close the connection:
    client.flush();
    client.stop();
    
  }//end client
  
  //*******socket
/*
  if(Serial.available()) {
    if(Serial.read() == 'r') ShowSockStatus();    
  }
  
  */
}

/*********************************/
boolean loggedIn() 
{
   webParser.clearBuffer(param_value,queryMax);
   //going to need a parse cookie function
   webParser.parseQuery(queryBuffer, "ARDUINOSESSIONID", param_value);
  
   if(arduinoSession == atol(param_value)) {
      return true;
   } else {
      return false; 
   }
}

void renderHtmlPage(char *page, WiFiClient client) 
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
             client.println(F("Connection: close"));
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
               client.write((const uint8_t *)tBuf, myFile.position() - lastPosition);
               lastPosition = myFile.position();
    
             }
              
            myFile.close();
             // disconnect the SD card

        } else {
          Serial.println("file not found"); 
          Serial.println(page);
           client.println(F("HTTP/1.1 404 Not Found"));
           client.println(F("Content-Type: text/html"));
           client.println(F("Connection: close"));
           client.println();
           client.println(F("<h2>File Not Found!</h2>"));
        }
        
       
        
}

void successAjax(WiFiClient client) {
       //ajax requires a response or will throw a connection lost
      client.println(F("HTTP/1.1 200 OK"));
      client.println(F("Content-Type: text/xml"));
      //client.println(F("Connection: close"));
      client.println();
      client.println(F("<?xml version = \"1.0\" ?>"));
      client.print(F("<response>success</response>"));
      Serial.println("success ajax");
}

void errorAjax(WiFiClient client) {
        client.println(F("HTTP/1.1 200 OK"));
        client.println(F("Content-Type: text/xml"));
        //client.println(F("Connection: close"));
        client.println();
        client.println(F("<?xml version = \"1.0\" ?>"));
        client.print(F("<response>error</response>"));
}

void parseReceivedRequest(WiFiClient client) 
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
       //hashpass.toCharArray(loginpass, 41); //convert the String to char string loginpass 
       
      if(webParser.compare(username,user) && webParser.compare(loginpass,pass)) {
          
        //Serial.println("logged inxxxxx ");
          arduinoSession = millis();
          Serial.println(arduinoSession);
          client.println(F("HTTP/1.1 200 OK"));
          client.println(F("Content-Type: text/xml"));
          client.print(F("Set-cookie: ARDUINOSESSIONID="));
          client.print(arduinoSession);
          client.println(F("; HttpOnly"));
          client.println(F("Connection: close"));
          client.println();
          client.println(F("<?xml version = \"1.0\" ?>"));
          client.print(F("<response>login</response>"));
        
      } else {
          client.println(F("HTTP/1.1 200 OK"));
          client.println(F("Content-Type: text/xml"));
          client.println(F("Connection: close"));
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
      client.println(F("Connection: close"));
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
            // char code[20];
            
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
      // ***************** set ada PWM 8 channels  ********************
    else if(webParser.contains(queryBuffer, "set8adapwms"))
    {

      set8AdaPWMs(client);

        
    } 
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
          client.println(F("Connection: close"));
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
          client.println(F("Connection: close"));
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
         
          webParser.clearBuffer(param_value, queryMax);
          webParser.parseQuery(queryBuffer, "gettemplate", param_value);
   
          Device *device = deviceDelegate.findDevice(atoi(param_value));

          webParser.clearBuffer(param_value,30);
          strcpy(param_value, device->getClassName());
          
          client.println(F("HTTP/1.1 200 OK"));
          client.println(F("Content-Type: text/xml"));
          client.println(F("Connection: close"));
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
          client.println(F("Connection: close"));
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
          
            if(webParser.compare(param_value, "Relay") || webParser.compare(param_value, "ArduinoRelay")) {
                
              relayAjaxOutput(client, device);
              
            } else if(webParser.compare(param_value, "OneWireSensor")) 
            {
               
              oneWireAjaxOutput(client, device);
               
            } 
            else if(webParser.compare(param_value, "PWM4")) 
            {
               pwm4AjaxOutput(client, device);
            } 
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
          device->setPin(convertPin(param_value));
          
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
          else if(webParser.compare(param_value, "ArduinoRelay"))
          {
               saveRelay(device);
          
          } 
          else if(webParser.compare(param_value, "OneWireSensor")) 
          {
           
               saveOneWire(device);
           
          } 
          else if(webParser.compare(param_value, "PWM4")) 
          {
               savePWM4(device);
          }  
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
          else if(webParser.compare(param_value, "ArduinoRelay")) {
              createArduinoRelay();
          } 
          else if(webParser.compare(param_value, "AdaFruitPWM8")) {
              createAdaFruit8PWM();
          }
          else if(webParser.compare(param_value, "Alert")) {
              createAlert();
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

int freeMemory() {
  
  return ESP.getFreeHeap();
  
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  //Serial.print("SSID: ");
  //Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

}


//********************** socket ********************

void ShowSockStatus() {
  /*
  for(int x = 0; x < MAX_SOCK_NUM; x++) {
    Serial.print(x);
    Serial.print(" - ");
    Serial.print(WiFi._state[x]);    
    Serial.print("  ");
    Serial.println(WiFi._server_port[x]);    
  }
  */
}



