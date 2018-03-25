// © 2noodles llc
// minh@2noodles.com

void set8AdaPWMs(WiFiClient client) {
      webParser.clearBuffer(param_value, queryMax);
      webParser.parseQuery(queryBuffer, "set8adapwms", param_value);
      //don't perform if it's not a device
      if(atoi(param_value) > 0) {
        Device *device = deviceDelegate.findDevice(atoi(param_value));
        char red[5] = {'\0'};
        char green[5] = {'\0'};
        char blue[5] = {'\0'};
        char white[5] = {'\0'};
        char aux1[5] = {'\0'};
        char aux2[5] = {'\0'};
        char aux3[5] = {'\0'};
        char aux4[5] = {'\0'};
        
        webParser.parseQuery(queryBuffer, "red", red);
        
        webParser.parseQuery(queryBuffer, "green", green);
        
        webParser.parseQuery(queryBuffer, "blue", blue);
        
        webParser.parseQuery(queryBuffer, "white", white);
        
        webParser.parseQuery(queryBuffer, "aux1", aux1);
        
        webParser.parseQuery(queryBuffer, "aux2", aux2);
        
        webParser.parseQuery(queryBuffer, "aux3", aux3);
        
        webParser.parseQuery(queryBuffer, "aux4", aux4);
        
        
        static_cast<AdaFruitPWM8*>(device)->setPWMs(atoi(red),atoi(green),atoi(blue),atoi(white),atoi(aux1),atoi(aux2),atoi(aux3),atoi(aux4) );
        
        successAjax(client);
      }
}



void adaFruit8PWMAjaxOutput(WiFiClient client, Device *device) {
           client.print(F("<pin>"));
          client.print(device->getPin());
          client.print(F("</pin>"));
            
          client.print(F("<dependent>"));
          client.print(device->getDependentDevice());
          client.print(F("</dependent>"));

          int i2cArray[2];
          static_cast<AdaFruitPWM8*>(device)->getI2C(i2cArray);
          client.print(F("<sda>"));
          client.print(i2cArray[0]);
          client.print(F("</sda>"));
          client.print(F("<scl>"));
          client.print(i2cArray[1]);
          client.print(F("</scl>"));
          // slider vals are in the events 
          client.print(F("<event>"));
          char eventstring[queryMax];
          static_cast<AdaFruitPWM8*>(device)->getEvent(eventstring);
//Serial.println(">>>> to html >>>>>>");
 //Serial.println(eventstring);
          client.print(eventstring);
          client.print(F("</event>"));
          int pinArray[8];
          static_cast<AdaFruitPWM8*>(device)->getPins(pinArray);

          client.print(F("<redpin>"));
          client.print(pinArray[0]);
          client.print(F("</redpin>"));
          client.print(F("<greenpin>"));
          client.print(pinArray[1]);
          client.print(F("</greenpin>"));
          client.print(F("<bluepin>"));
          client.print(pinArray[2]);
          client.print(F("</bluepin>"));
          client.print(F("<whitepin>"));
          client.print(pinArray[3]);
          client.print(F("</whitepin>"));
         client.print(F("<aux1>"));
          client.print(pinArray[4]);
          client.print(F("</aux1>"));
          client.print(F("<aux2>"));
          client.print(pinArray[5]);
          client.print(F("</aux2>"));
          client.print(F("<aux3>"));
          client.print(pinArray[6]);
          client.print(F("</aux3>"));
          client.print(F("<aux4>"));
          client.print(pinArray[7]);
          client.print(F("</aux4>"));
       Serial.println("ada8 output");
}



//save ada8
void saveAdaFruit8PWM(Device *device) {

            webParser.clearBuffer(param_value, queryMax);
            webParser.parseQuery(queryBuffer, "sda", param_value);
            int sda;
            if(param_value[0] == '\0') {
              sda = UNSET;
            } else {
              sda = convertPin(param_value);
            }

             webParser.clearBuffer(param_value, queryMax);
            webParser.parseQuery(queryBuffer, "scl", param_value);
            int scl;
            if(param_value[0] == '\0') {
              scl = UNSET;
            } else {
              scl = convertPin(param_value);
            }

            static_cast<AdaFruitPWM8*>(device)->setI2C(sda, scl);
            
            webParser.clearBuffer(param_value, queryMax);
            webParser.parseQuery(queryBuffer, "redpin", param_value);
            int red;
            if(param_value[0] == '\0') {
              red = UNSET;
            } else {
              red = atoi(param_value);
            }
            
            webParser.clearBuffer(param_value, queryMax);
            webParser.parseQuery(queryBuffer, "greenpin", param_value);
            int green;
            if(param_value[0] == '\0') {
              green = UNSET;
            } else {
              green = atoi(param_value);
            }
            
            
            webParser.clearBuffer(param_value, queryMax);
            webParser.parseQuery(queryBuffer, "bluepin", param_value);
            int blue;
            if(param_value[0] == '\0') {
              blue = UNSET;
            } else {
              blue = atoi(param_value);
            }
            
            webParser.clearBuffer(param_value, queryMax);
            webParser.parseQuery(queryBuffer, "whitepin", param_value);
            int white;
 
            if(param_value[0] == '\0') {
              white = UNSET;
            } else {
              white = atoi(param_value);
            }
            
             webParser.clearBuffer(param_value, queryMax);
            webParser.parseQuery(queryBuffer, "aux1pin", param_value);
            int aux1;
            if(param_value[0] == '\0') {
              aux1 = UNSET;
            } else {
              aux1 = atoi(param_value);
            }
            
            webParser.clearBuffer(param_value, queryMax);
            webParser.parseQuery(queryBuffer, "aux2pin", param_value);
            int aux2;
            if(param_value[0] == '\0') {
              aux2 = UNSET;
            } else {
              aux2 = atoi(param_value);
            }
            
            
            webParser.clearBuffer(param_value, queryMax);
            webParser.parseQuery(queryBuffer, "aux3pin", param_value);
            int aux3;
            if(param_value[0] == '\0') {
              aux3 = UNSET;
            } else {
              aux3 = atoi(param_value);
            }
            
            webParser.clearBuffer(param_value, queryMax);
            webParser.parseQuery(queryBuffer, "aux4pin", param_value);
            int aux4;
 
            if(param_value[0] == '\0') {
              aux4 = UNSET;
            } else {
              aux4 = atoi(param_value);
            }
            
            
            static_cast<AdaFruitPWM8*>(device)->setPins(red,green,blue,white,aux1,aux2,aux3,aux4);
            
            webParser.clearBuffer(param_value, queryMax);
            webParser.parseQuery(queryBuffer, "dependent", param_value);
            device->setDependentDevice(atoi(param_value));
            
            webParser.clearBuffer(param_value, queryMax);
            webParser.parseQuery(queryBuffer, "event", param_value);
         //Serial.println("********");
         //Serial.println(param_value);
            device->setEvent(param_value);
            
            Serial.println("ada8 saved");
}



//create ada8 - lock server if you dont create event?
void createAdaFruit8PWM() {
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "name", param_value);
  //Serial.println(queryBuffer);
        char tempDependent[5];
        webParser.parseQuery(queryBuffer, "dependent", tempDependent);

        int sda, scl;
        
        char tempSDA[5] = {};
        webParser.parseQuery(queryBuffer, "sda", tempSDA);

        if(tempSDA[0] == '\0') {
          sda = UNSET;
        } else {
          sda = convertPin(tempSDA);
        }
        
        char tempSCL[5] = {};
        webParser.parseQuery(queryBuffer, "scl", tempSCL);

        if(tempSCL[0] == '\0') {
          scl = UNSET;
        } else {
          scl = convertPin(tempSCL);
        }
        
        
        //***********create device
        AdaFruitPWM8 *temp = new AdaFruitPWM8(param_value, atoi(tempDependent), sda, scl);
        deviceDelegate.addDevice( temp );
        //***********
        
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "image", param_value);
        deviceDelegate.currentDevice()->setImageName(param_value);
        
        char color[7];
        webParser.parseQuery(queryBuffer, "color", param_value);
        findColor(param_value, color);
        deviceDelegate.currentDevice()->setDeviceColor(color);
/*
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "sdl", param_value);
        int sdl;
        if(param_value[0] == '\0') {
          sdl = UNSET;
        } else {
          sdl = atoi(param_value);
        }

         webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "clk", param_value);
        int clk;
        if(param_value[0] == '\0') {
          clk = UNSET;
        } else {
          clk = atoi(param_value);
        }

        static_cast<AdaFruitPWM8*>(temp)->setI2C(sdl, clk);
   */         
        
         webParser.clearBuffer(param_value, queryMax);
            webParser.parseQuery(queryBuffer, "redpin", param_value);
            int red;
            if(param_value[0] == '\0') {
              red = UNSET;
            } else {
              red = atoi(param_value);
            }
            
            webParser.clearBuffer(param_value, queryMax);
            webParser.parseQuery(queryBuffer, "greenpin", param_value);
            int green;
            if(param_value[0] == '\0') {
              green = UNSET;
            } else {
              green = atoi(param_value);
            }
            
            
            webParser.clearBuffer(param_value, queryMax);
            webParser.parseQuery(queryBuffer, "bluepin", param_value);
            int blue;
            if(param_value[0] == '\0') {
              blue = UNSET;
            } else {
              blue = atoi(param_value);
            }
            
            webParser.clearBuffer(param_value, queryMax);
            webParser.parseQuery(queryBuffer, "whitepin", param_value);
            int white;
 
            if(param_value[0] == '\0') {
              white = UNSET;
            } else {
              white = atoi(param_value);
            }
            
             webParser.clearBuffer(param_value, queryMax);
            webParser.parseQuery(queryBuffer, "aux1pin", param_value);
            int aux1;
            if(param_value[0] == '\0') {
              aux1 = UNSET;
            } else {
              aux1 = atoi(param_value);
            }
            
            webParser.clearBuffer(param_value, queryMax);
            webParser.parseQuery(queryBuffer, "aux2pin", param_value);
            int aux2;
            if(param_value[0] == '\0') {
              aux2 = UNSET;
            } else {
              aux2 = atoi(param_value);
            }
            
            
            webParser.clearBuffer(param_value, queryMax);
            webParser.parseQuery(queryBuffer, "aux3pin", param_value);
            int aux3;
            if(param_value[0] == '\0') {
              aux3 = UNSET;
            } else {
              aux3 = atoi(param_value);
            }
            
            webParser.clearBuffer(param_value, queryMax);
            webParser.parseQuery(queryBuffer, "aux4pin", param_value);
            int aux4;
 
            if(param_value[0] == '\0') {
              aux4 = UNSET;
            } else {
              aux4 = atoi(param_value);
            }
            
            
            static_cast<AdaFruitPWM8*>(temp)->setPins(red,green,blue,white,aux1,aux2,aux3,aux4);
            

        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "event", param_value);

        deviceDelegate.currentDevice()->setEvent(param_value);
        /*
        if(param_value[0] == '\0') {
          Serial.println("event is a null xxxxxxxxxx");
          if(param_value != NULL) {
            Serial.println("param_value is not NULL");
          }
          //deviceDelegate.currentDevice()->setHasEvent(false);
        } else {
          deviceDelegate.currentDevice()->setEvent(param_value);
        }
        
        Serial.println("ada8 created");
        */

}

