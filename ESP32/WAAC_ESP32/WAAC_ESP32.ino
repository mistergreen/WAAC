
// © 2noodles llc
// minh@2noodles.com
/* NOTES
 *  
 * analogWrite(led, brightness) - not implemented in ESP32 core yet, use LEDC - 15bit max? 13 bit-8191 2^13-1
 * AnalogInput can be converted to digital except pin 34-39
 * 
 */
#define MAXDEVICE   20 //maximum number you can add devices objects. Also defined in deviceDelegate
#define UNSET       -1
#define bufferMax   628
#define queryMax    350

// Define SPIFFS to use it
#define USE_SPIFFS

#include <ArduinoJson.h>

#include <WiFi.h>
#include <WiFiUdp.h>

#include <SPI.h>
#include <Servo.h>
#include <TimeLib.h>
#include <Wire.h>


// Load the selected file system
#ifdef USE_SPIFFS
  #include <SPIFFS.h>
  #define FS_HANDLER SPIFFS
  #define FS_PARAM   true
#else
  #include <SD.h>
  #define FS_HANDLER SD
  #define FS_PARAM   5 // SS pin
#endif

//devices - add any necessary libs for devices here!
//for generic device using digial pins or analog
#include "Sensor.h"
#include "Device.h"

//don't forget to include dependent library into the ino file

#include "Adafruit_PWMServoDriver.h"
#include "AdaFruitPWM8.h"
#include "Alert.h"
#include "Analog.h"
#include "DeviceDelegate.h"
#include "HallSensor.h"
#include "Input.h"
#include "InputMCP.h"
#include "MCPhelper.h"
#include "OneWire.h"
#include "OneWireSensor.h"
#include "PCAhelper.h"
#include "PWM4.h"
#include "rBase64.h"
#include "Relay.h" 
#include "RelayMCP.h" 
#include "RelayPCA.h" 
#include "ServoMotor.h"
#include "Shunt.h" 
#include "Video.h"
#include "WebParser.h"
#include "WWWsettings.h"

/************* time **************/


const char *dayName[8] = {"", "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

const char *monthName[13] = {"", "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};


/************* Device menu **************/
// { device type (must be unique & same as classType/Name), description, html form to configure it }
const char *deviceMenu[14][3] = {
                        {"AdaFruitPWM8","PCA9685 PWM 12-bit, 8 channel", "adapwm8.htm"},
                        {"Alert","Email Alerts", "alert.htm"},
                        {"Analog","Analog Sensors", "analog.htm"},
                        {"HallSensor","Flow Hall Sensors", "hall.htm"},
                        {"Input","Native Digital Input", "input.htm"},
                        {"InputMCP","MCP23017 Digital Input", "input_i2c.htm"},
                        {"OneWireSensor", "OneWire Dallas/Maxim", "onewire.htm"},
                        {"PWM4","ESP PWM, 4 channels", "pwm4.htm"},
                        {"Relay","Native Digital out", "relay.htm"},
                        {"RelayPCA","PCA9685 Digital out", "relay_i2c.htm"},
                        {"RelayMCP","MCP23017 Digital out", "relay_i2c.htm"},
                        {"ServoMotor","Servo Motor out", "servo.htm"},
                        {"Video", "Yout-tube Stream", "video.htm"},
                        {NULL}
                       };

const char *deviceStyle[12][2] = {
                        {"none.jpg", "FFFFFF"},
                        {"alert.jpg", "FCDF31"},
                        {"fan.jpg", "4B8BE3"},
                        {"flow.gif", "70B829"},
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

// Our configuration structure.
//
// Never use a JsonDocument to store the configuration!
// A JsonDocument is *not* a permanent storage; it's only a temporary storage
// used during the serialization phase. See:
// https://arduinojson.org/v6/faq/why-must-i-create-a-separate-config-object/
struct Config {
  char ssid[64];
  char password[64];
  int ip[4];
  int gateway[4];
  int subnet[4];
  int timeZone;
  char ntpServer[32];
};

// Initialize the Ethernet server library
// with the IP address and port you want to use 
// (port 80 is default for HTTP):
WiFiServer server(80);

int status = WL_IDLE_STATUS;
int bufferSize;
char queryBuffer[bufferMax];
char param_value[queryMax];
int timeArray[6];
WebParser webParser;

File webFile;
WWWsettings wwws;

/************* website login **************/

// site login
char username[] = "admin";
char loginpass[] = "waac";


unsigned long arduinoSession = 1;


const char *configFile = "/settings.json";  // <- SD library uses 8.3 filenames
Config config;                         // <- global configuration object

const char *devicesFile = "/devices.json";

/****************************************** sketch Logic **********************************************************/
// Compute the required size
// 25 expected fields + names with 15 characters + events (200 max chars TO BE CHECKED!) + 4 additional strings of 64 characters (servo motor example).
// The size may need to be enlarged for other objects.
static const int sDEVICES_DOC_SIZE = JSON_ARRAY_SIZE(11) + JSON_OBJECT_SIZE(1) + (JSON_OBJECT_SIZE(25) + 200 + 4 * 64)*2;

// Allocate the JsonDocument
// Don't forget to change the capacity to match your requirements.
// Use arduinojson.org/assistant to compute the capacity.
StaticJsonDocument<sDEVICES_DOC_SIZE> doc;

void setup() 
{
  // start serial port:
  Serial.begin(115200);

  //************ initialize SD card *******************

#ifdef USE_SPIFFS
  Serial.println ("Initializing SPIFFS...");
#else
  Serial.println ("Initializing SD card...");
#endif 

  if (!FS_HANDLER.begin(FS_PARAM)) {
        Serial.println("ERROR - File system initialization failed!");
        return;    // init failed
  }
  //Serial.println("SUCCESS - SD card initialized.");
    
  // check for index.htm file
  //it's picky with file extension, only 3 letters
  if (!FS_HANDLER.exists("/index.htm")) {
        Serial.println("ERROR - Can't find index.htm file!");
        return;  // can't find index file
  }
  Serial.println("SUCCESS - Found index.htm file.");

  // Dump config file
  Serial.println(F("Print config file..."));
  printFile(configFile);

  // Should load default config if run for the first time
  Serial.println(F("Loading configuration..."));
  loadConfiguration(configFile, config);

  // Set the IP from the configuration
  IPAddress ip(config.ip[0], config.ip[1], config.ip[2], config.ip[3]);
  IPAddress gateway(config.gateway[0], config.gateway[1], config.gateway[2], config.gateway[3]); 
  IPAddress subnet(config.subnet[0], config.subnet[1], config.subnet[2], config.subnet[3]);
  //IPAddress primaryDNS(8, 8, 8, 8); //optional
  //IPAddress secondaryDNS(8, 8, 4, 4); //optional


  //************ wifi *******************
  // Static IP doesn't look like it works for the ESP32 at this point
  //if (!WiFi.config(ip, gateway, subnet, primaryDNS, secondaryDNS)) {

  if (!WiFi.config(ip, gateway, subnet, gateway)) {
    Serial.println("STA Failed to configure");
  }

  Serial.print("Connecting to ");
  Serial.println(config.ssid);

  WiFi.begin(config.ssid, config.password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("Connected to wifi");
  printWifiStatus();

  // Load all stored devices.
  loadDevices(devicesFile);

  // set up server for the rest of the sockets
  server.begin();
  
  //set up upd first so it gets sockets #0
  wwws.begin(config.timeZone, config.ntpServer);
 
  //delay(10000);
  
  //sync time to NTP
  wwws.syncNTP();
  
  Serial.print("memory ");
  Serial.println(freeMemory());
}


int convertPin(char *pin){
  //Serial.print("pin: ");
  //Serial.println(pin);
  if(webParser.contains(pin, "A0")) {
    return A0;
  } else if(webParser.contains(pin, "A3")) {
    return A3;
  } else if(webParser.contains(pin, "A4")) {
    return A4;
  } else if(webParser.contains(pin, "A5")) {
    return A5;
  } else if(webParser.contains(pin, "A6")) {
    return A6;
  } else if(webParser.contains(pin, "A7")) {
    return A7;
  } else if(webParser.contains(pin, "A18")) {
    return A18;
  } else if(webParser.contains(pin, "A19")) {
    return A19;
  } else if(webParser.contains(pin, "A14")) {
    return A14;
  } else if(webParser.contains(pin, "A15")) {
    return A15;
  } else if(webParser.contains(pin, "A16")) {
    return A16;
  } else if(webParser.contains(pin, "A17")) {
    return A17;
  } 
   
    //Serial.print("can't find pin: ");
    //Serial.println(atoi(pin));
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
    
     bool currentLineIsBlank = true;
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
bool loggedIn() 
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
        char file[25];

        // If the file is not starting with a / then add it at the beginning.
        if (page[0] != '/') {
          strcpy(file, "/");
        }
        else {
          strcpy(file, "");
        }

        // Then add the page name.
        strcat(file, page);
        
        File myFile = FS_HANDLER.open(file);        // open web page file
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
          Serial.println(file);
           client.println(F("HTTP/1.1 404 Not Found"));
           client.println(F("Content-Type: text/html"));
           client.println(F("Connection: close"));
           client.println();
           client.print(file);
           client.println(F("<h2> File Not Found!</h2>"));
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
        //Serial.print("not logged in");
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
    // ***************** list devices in index.htm  ************************************   
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
        client.print(deviceDelegate.devices[i]->read());
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

          if(deviceDelegate.devices[i]->isSensor()) {
            client.print(F("<unit>"));
            client.print( deviceDelegate.devices[i]->getUnit() );
            client.print(F("</unit>"));
            client.print(F("<min>"));
            client.print( deviceDelegate.devices[i]->getMin() );
            client.print(F("</min>"));
            client.print(F("<max>"));
            client.print( deviceDelegate.devices[i]->getMax() );
            client.print(F("</max>"));
            
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
      saveSetting(client, config);
        
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
            
            //Serial.println(deviceMenu[i][0]);
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
          
            if(webParser.compare(param_value, "Relay")) {
                
              relayAjaxOutput(client, device);
       
            } 
            else if(webParser.compare(param_value, "RelayPCA")) 
            {
               
              relayPCAajaxOutput(client, device);
               
            } 
            else if(webParser.compare(param_value, "RelayMCP")) 
            {
               Serial.println("crash1");
              relayMCPajaxOutput(client, device);
               
            } 
            else if(webParser.compare(param_value, "OneWireSensor")) 
            {
               
              oneWireAjaxOutput(client, device);
               
            } 
            else if(webParser.compare(param_value, "Input")) 
            {
               
              inputAjaxOutput(client, device);
               
            } 
            else if(webParser.compare(param_value, "InputMCP")) 
            {
               
              inputMCPajaxOutput(client, device);
               
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
            else if(webParser.compare(param_value, "HallSensor")) 
            {
               hallAjaxOutput(client, device);
                
            }
            else if(webParser.compare(param_value, "ServoMotor")) 
            {
               servoAjaxOutput(client, device);
                
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
          //Serial.println("xxxxxxx save device xxxxxxxx");
          //alter existing device
          Device *device = deviceDelegate.findDevice(atoi(param_value));
   
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
          
          //no need for Input - the top params cover it all
          
          webParser.clearBuffer(param_value, queryMax);
          webParser.parseQuery(queryBuffer, "devicetype", param_value);
          
          if(webParser.compare(param_value, "Relay"))
          {
               saveRelay(device);
          
          } 
          else if(webParser.compare(param_value, "RelayPCA"))
          {
               saveRelayPCA(device);
          
          } 
           else if(webParser.compare(param_value, "InputMCP"))
          {
               saveInputMCP(device);
          
          } 
          else if(webParser.compare(param_value, "RelayMCP"))
          {
               saveRelayMCP(device);
          
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
          else if(webParser.compare(param_value, "HallSensor")) 
          {
             saveHallSensor(device);
            
          }
          else if(webParser.compare(param_value, "ServoMotor")) 
          {
             saveServoMotor(device);
            
          }
          //turn / reset devices back
          device->setSuspendTime(false);
          
        } else {
          //****************************************create new object ******************************
          //Serial.println("xxxxxxx created device xxxxxxxx");
          //don't allow more than 10 devices
          if(deviceDelegate.getDeviceCount() >= MAXDEVICE) return;
          
          webParser.clearBuffer(param_value, queryMax);
          webParser.parseQuery(queryBuffer, "devicetype", param_value);
          //******************* RELAY ***********************
          if(webParser.compare(param_value, "Relay")) {
              createRelay();
          } 
          else if(webParser.compare(param_value, "Input")) {
              createInput();
          }
          else if(webParser.compare(param_value, "InputMCP")) {
              createInputMCP();
          }
          else if(webParser.compare(param_value, "RelayPCA")) {
              createRelayPCA();
          }
          else if(webParser.compare(param_value, "RelayMCP")) {
              createRelayMCP();
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
          else if(webParser.compare(param_value, "HallSensor")) {
              createHallSensor();
          }
          else if(webParser.compare(param_value, "ServoMotor")) {
              createServoMotor();
          }
        }
        
        saveDevices(devicesFile);
        
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
