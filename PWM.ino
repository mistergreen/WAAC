// © 2noodles llc
// minh@2noodles.com

void set4PWM(NetworkClient client) {
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

void pwm4AjaxOutput(NetworkClient client, Device *device) {
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


void createPWM4() {
       

        //***********create device
        PWM4 *temp = new PWM4();
        deviceDelegate.addDevice( temp );
        //***********
        
        JsonObject obj = doc.as<JsonObject>();

        deviceDelegate.currentDevice()->deserialize(obj);

        char color[7];
        char imageName[30];
        strcpy(imageName, obj["imageName"]);
        findColor(imageName, color);
        deviceDelegate.currentDevice()->setDeviceColor(color);
}
