// © 2noodles llc
// minh@2noodles.com

void set4PWM(WiFiClient client) {
   webParser.clearBuffer(param_value, queryMax);
      webParser.parseQuery(queryBuffer, "setpwms", param_value);

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

void pwm4AjaxOutput(WiFiClient client, Device *device) {
        client.print(F("<pin>"));
        client.print(device->getPin());
        client.print(F("</pin>"));
            
        client.print(F("<dependent>"));
        client.print(device->getDependentDevice());
        client.print(F("</dependent>"));
        client.print(F("<event>"));
        char string[queryMax];
        device->getEvent(string);
        client.print(string);
        client.print(F("</event>"));
        int pinArray[4];
        static_cast<PWM4*>(device)->getPins(pinArray);
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
        
        
        static_cast<PWM4*>(device)->setPins(red,green,blue,white);
        
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "dependent", param_value);
        device->setDependentDevice(convertPin(param_value));
        
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "event", param_value);
        device->setEvent(param_value);
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
        //Serial.print("red:");
        //Serial.print(red);
        
        static_cast<PWM4*>(temp)->setPins(red,green,blue,white);
       
        
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "event", param_value);
        static_cast<PWM4*>(temp)->setEvent(param_value);
        
  
}



