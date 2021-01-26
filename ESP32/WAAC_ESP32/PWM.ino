// © 2noodles llc
// minh@2noodles.com

void set4PWM(WiFiClient client) {
  //use to set brightness
      webParser.clearBuffer(param_value, queryMax);
      webParser.parseQuery(queryBuffer, "setpwms", param_value);
      //don't perform if it's not a device
      
      if(atoi(param_value) > 0) {
        Device *device = deviceDelegate.findDevice(atoi(param_value));
        char red[4] = {'\0'};
        char green[4] = {'\0'};
        char blue[4] = {'\0'};
        char white[4] = {'\0'};
    
        webParser.parseQuery(queryBuffer, "red", red);
        
        webParser.parseQuery(queryBuffer, "green", green);
        
        webParser.parseQuery(queryBuffer, "blue", blue);
        
        webParser.parseQuery(queryBuffer, "white", white);
  
        
        static_cast<PWM4*>(device)->setPWMs(atoi(red),atoi(green),atoi(blue),atoi(white));
        
        successAjax(client);
      }
}

void pwm4AjaxOutput(WiFiClient client, Device *device) {
        client.print(F("<pin>"));
        client.print(device->getPin());
        client.print(F("</pin>"));
            
        client.print(F("<dependent>"));
        client.print(static_cast<PWM4*>(device)->getDependentDevice());
        client.print(F("</dependent>"));
        client.print(F("<event>"));
        char string[queryMax] = {'\0'};
        static_cast<PWM4*>(device)->getEvent(string);
        client.print(string);
        client.print(F("</event>"));
        client.print(F("<eventColors>"));
        char colorsString[queryMax] = {'\0'};
        static_cast<PWM4*>(device)->getEventColors(colorsString);
        client.print(colorsString);
        client.print(F("</eventColors>"));
        int pinArray[4], channelArray[4];
        Serial.println("get PWM4 arduino pins");
        static_cast<PWM4*>(device)->getPins(pinArray,channelArray);
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

        client.print(F("<redChannel>"));
        client.print(channelArray[0]);
        client.print(F("</redChannel>"));
        client.print(F("<greenChannel>"));
        client.print(channelArray[1]);
        client.print(F("</greenChannel>"));
        client.print(F("<blueChannel>"));
        client.print(channelArray[2]);
        client.print(F("</blueChannel>"));
        client.print(F("<whiteChannel>"));
        client.print(channelArray[3]);
        client.print(F("</whiteChannel>"));
}

void savePWM4(Device *device) {

        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "redpin", param_value);
        int red;
        if(param_value[0] == '\0') {
          red = UNSET;
        } else {
          red = convertPin(param_value);
        }
        
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "greenpin", param_value);
        int green;
        if(param_value[0] == '\0') {
          green = UNSET;
        } else {
          green = convertPin(param_value);
        }
            
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "bluepin", param_value);
        int blue;
        if(param_value[0] == '\0') {
          blue = UNSET;
        } else {
          blue = convertPin(param_value);
        }
        
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "whitepin", param_value);
        int white;
 
        if(param_value[0] == '\0') {
          white = UNSET;
        } else {
          white = convertPin(param_value);
        }

        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "redChannel", param_value);
        
        int redChannel;
        if(param_value[0] == '\0') {
          redChannel = UNSET;
        } else {
          redChannel = atoi(param_value);
        }
        
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "greenChannel", param_value);
        int greenChannel;
        if(param_value[0] == '\0') {
          greenChannel = UNSET;
        } else {
          greenChannel = atoi(param_value);
        }
            
        
        
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "blueChannel", param_value);
        int blueChannel;
        if(param_value[0] == '\0') {
          blueChannel = UNSET;
        } else {
          blueChannel = atoi(param_value);
        }
        
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "whiteChannel", param_value);
        int whiteChannel;
 
        if(param_value[0] == '\0') {
          whiteChannel = UNSET;
        } else {
          whiteChannel = atoi(param_value);
        }
        
        
        static_cast<PWM4*>(device)->setPins(red,green,blue,white, redChannel,greenChannel,blueChannel,whiteChannel);
        
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "dependent", param_value);
        static_cast<PWM4*>(device)->setDependentDevice(convertPin(param_value));
        
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "event", param_value);
        static_cast<PWM4*>(device)->setEvent(param_value);

        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "eventColors", param_value);
        static_cast<PWM4*>(device)->setEventColors(param_value);
}

void createPWM4() {
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "name", param_value);
        
         char tempDependent[5];
        webParser.parseQuery(queryBuffer, "dependent", tempDependent);

        //***********create device
        PWM4 *temp = new PWM4(param_value, atoi(tempDependent));
        deviceDelegate.addDevice( temp );
        //***********
        
        //Serial.println(queryBuffer);
        
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "image", param_value);
        deviceDelegate.currentDevice()->setImageName(param_value);
        
        char color[7];
        webParser.parseQuery(queryBuffer, "color", param_value);
        findColor(param_value, color);
        deviceDelegate.currentDevice()->setDeviceColor(color);
        

        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "redpin", param_value);
        int red;
        if(param_value[0] == '\0') {
          red = UNSET;
        } else {
          red = convertPin(param_value);
        }
        
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "greenpin", param_value);
        int green;
        if(param_value[0] == '\0') {
          green = UNSET;
        } else {
          green = convertPin(param_value);
        }
            
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "bluepin", param_value);
        int blue;
        if(param_value[0] == '\0') {
          blue = UNSET;
        } else {
          blue = convertPin(param_value);
        }
        
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "whitepin", param_value);
        int white;
 
        if(param_value[0] == '\0') {
          white = UNSET;
        } else {
          white = convertPin(param_value);
        }
         webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "redChannel", param_value);
        
        int redChannel;
        if(param_value[0] == '\0') {
          redChannel = UNSET;
        } else {
          redChannel = atoi(param_value);
        }
        
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "greenChannel", param_value);
        int greenChannel;
        if(param_value[0] == '\0') {
          greenChannel = UNSET;
        } else {
          greenChannel = atoi(param_value);
        }
            
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "blueChannel", param_value);
        int blueChannel;
        if(param_value[0] == '\0') {
          blueChannel = UNSET;
        } else {
          blueChannel = atoi(param_value);
        }
        
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "whiteChannel", param_value);
        int whiteChannel;
 
        if(param_value[0] == '\0') {
          whiteChannel = UNSET;
        } else {
          whiteChannel = atoi(param_value);
        }
   
        static_cast<PWM4*>(temp)->setPins(red,green,blue,white,redChannel,greenChannel,blueChannel,whiteChannel);

        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "event", param_value);
        static_cast<PWM4*>(temp)->setEvent(param_value);
        
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "eventColors", param_value);
        static_cast<PWM4*>(temp)->setEventColors(param_value);
}
