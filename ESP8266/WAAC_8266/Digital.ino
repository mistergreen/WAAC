// © 2noodles llc
// minh@2noodles.com

void switchDigital(WiFiClient client) {
      webParser.clearBuffer(param_value, queryMax);
      webParser.parseQuery(queryBuffer, "switch", param_value);
       //generic casting works but not recommended
       //((Relay *)deviceDelegate.findDevice(atoi(param_value)))->togglePin();
       //static_cast<Relay*>(deviceDelegate.findDevice(atoi(param_value)))->toggleState();
      deviceDelegate.findDevice(atoi(param_value))->toggleState();
      //Serial.print("switch: ");
      //Serial.println(param_value);
      successAjax(client);
}

void relayAjaxOutput(WiFiClient client, Device *device) {
      client.print(F("<pin>"));
      client.print(device->getPin());
      client.print(F("</pin>"));
      client.print(F("<invert>"));
      client.print(static_cast<Relay*>(device)->getInvert());
      client.print(F("</invert>"));
      
      client.print(F("<dependent>"));
      client.print(device->getDependentDevice());
      client.print(F("</dependent>"));
      client.print(F("<event>"));
      char eventString[queryMax];
      static_cast<Relay*>(device)->getEvent(eventString);
      client.print(eventString);
      client.print(F("</event>"));
}

void createRelay() {

        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "name", param_value);
        
        char tempPin[5]={'\0'};
        webParser.parseQuery(queryBuffer, "pin", tempPin);
        
        char tempDependent[5];
        webParser.parseQuery(queryBuffer, "dependent", tempDependent);
        Serial.print("create pin: ");
        Serial.println(tempPin);
        //***********create device
        Relay *temp = new Relay(param_value, convertPin(tempPin), atoi(tempDependent));
        deviceDelegate.addDevice( temp );
        //***********

        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "image", param_value);
        deviceDelegate.currentDevice()->setImageName(param_value);
        
        char color[7];
        webParser.parseQuery(queryBuffer, "color", param_value);
        findColor(param_value, color);
        deviceDelegate.currentDevice()->setDeviceColor(color);

        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "event", param_value);
        deviceDelegate.currentDevice()->setEvent(param_value);


        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "invert", param_value);
        // must be 1 or 0
        static_cast<Relay*>(temp)->setInvert(atoi(param_value));
        
        /* eps8266 dosn't like this, cerashes
        
        if(webParser.compare(param_value, '\0')) {
          Serial.println("event is a null xxxxxxxxxx");
        } else {
          Serial.println("begin event");
          //delay(500);
          //deviceDelegate.currentDevice()->setEvent(param_value);
          //static_cast<Relay*>(temp)->setEvent(param_value);
        }
        */
        

}

void createArduinoRelay() {
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "name", param_value);
        
        char tempPin[5];
        webParser.parseQuery(queryBuffer, "pin", tempPin);
        
        char tempDependent[5];
        webParser.parseQuery(queryBuffer, "dependent", tempDependent);
        
        //***********create device
        ArduinoRelay *temp = new ArduinoRelay(param_value, convertPin(tempPin), atoi(tempDependent));
        deviceDelegate.addDevice( temp );
        //***********
        
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "image", param_value);
        deviceDelegate.currentDevice()->setImageName(param_value);
        
        char color[7];
        webParser.parseQuery(queryBuffer, "color", param_value);
        findColor(param_value, color);
        deviceDelegate.currentDevice()->setDeviceColor(color);

        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "event", param_value);

        deviceDelegate.currentDevice()->setEvent(param_value);

        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "invert", param_value);
        // must be 1 or 0
        static_cast<Relay*>(temp)->setInvert(atoi(param_value));
        
        /*
        if(webParser.compare(param_value, '\0')) {
          Serial.println("event is a null xxxxxxxxxx");
        } else {
          deviceDelegate.currentDevice()->setEvent(param_value);
        }
        */
        

}


void saveRelay(Device *device) {
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "dependent", param_value);
        device->setDependentDevice(atoi(param_value));
        
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "event", param_value);
        device->setEvent(param_value);

        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "invert", param_value);
        // must be 1 or 0
        static_cast<Relay*>(device)->setInvert(atoi(param_value));
        
}


