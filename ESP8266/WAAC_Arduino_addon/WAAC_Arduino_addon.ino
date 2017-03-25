#define queryMax 350

#include <WebParser.h>

char serialBuffer[queryMax];
int buffCount = 0;

char pin[3];
char value[3];

WebParser webParser;

void setup() {
  Serial.begin(115200);
  Serial3.begin(115200);

}

void loop() {
  if(Serial3.available()) {
       char inChar = (char)Serial3.read();
    
       if(buffCount < queryMax) {
          serialBuffer[buffCount++] = inChar;
       }
      
       if (inChar == '\n') {
          parseSerial();
       }

    }
/*
    if(Serial.available()) {
      char inChar2 = (char)Serial.read();
        if (inChar2 == 'z') {
          test();
       }
    }
*/

}

void parseSerial() {
    //Serial.println(serialBuffer);
     buffCount = 0;
     Serial.flush();
     
     webParser.clearBuffer(pin, 3);
     webParser.clearBuffer(value, 3);
     
     webParser.parseQuery(serialBuffer, "pin", pin);
     webParser.parseQuery(serialBuffer, "value", value);  

     if(pin[0] != '\0' && value[0] != '\0') {
 
         pinMode(atoi(pin), OUTPUT);
         digitalWrite(atoi(pin), atoi(value));
   
     }
  
    webParser.clearBuffer(serialBuffer, queryMax);
}

/*
void test() {
   Serial.print("lock");
}
*/



