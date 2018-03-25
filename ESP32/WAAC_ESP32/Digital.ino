// © 2noodles llc
// minh@2noodles.com

void switchDigital(WiFiClient client) {
      webParser.clearBuffer(param_value, queryMax);
      webParser.parseQuery(queryBuffer, "switch", param_value);
       //generic casting works but not recommended
       //((Relay *)deviceDelegate.findDevice(atoi(param_value)))->togglePin();
       //static_cast<Relay*>(deviceDelegate.findDevice(atoi(param_value)))->toggleState();
      //don't perform if it's not a device
      if(atoi(param_value) > 0) {
        deviceDelegate.findDevice(atoi(param_value))->toggleState();
        //Serial.print("switch: ");
        //Serial.println(param_value);
        successAjax(client);
      }
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

void relayPCAajaxOutput(WiFiClient client, Device *device) {
      client.print(F("<pin>"));
      client.print(device->getPin());
      client.print(F("</pin>"));
      client.print(F("<invert>"));
      client.print(static_cast<RelayPCA*>(device)->getInvert());
      client.print(F("</invert>"));

      int i2cArray[2];
      static_cast<RelayPCA*>(device)->getI2C(i2cArray);
      client.print(F("<sda>"));
      client.print(i2cArray[0]);
      client.print(F("</sda>"));
      client.print(F("<scl>"));
      client.print(i2cArray[1]);
      client.print(F("</scl>"));
          
      
      client.print(F("<dependent>"));
      client.print(device->getDependentDevice());
      client.print(F("</dependent>"));
      client.print(F("<event>"));
      char eventString[queryMax];
      static_cast<RelayPCA*>(device)->getEvent(eventString);
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
        //Serial.print("create pin: ");
        //Serial.println(tempPin);
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

}

void createRelayPCA() {

        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "name", param_value);
        
        char tempPin[5]={'\0'};
        webParser.parseQuery(queryBuffer, "pin", tempPin);
        
        char tempDependent[5];
        webParser.parseQuery(queryBuffer, "dependent", tempDependent);

        //***********create device
        Relay *temp = new RelayPCA(param_value, convertPin(tempPin), atoi(tempDependent));
        deviceDelegate.addDevice( temp );
        //***********

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

        static_cast<RelayPCA*>(temp)->setI2C(sda, scl);
    

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
        static_cast<RelayPCA*>(temp)->setInvert(atoi(param_value));

}

void createRelayMCP() {

        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "name", param_value);
        
        char tempPin[5]={'\0'};
        webParser.parseQuery(queryBuffer, "pin", tempPin);
        
        char tempDependent[5];
        webParser.parseQuery(queryBuffer, "dependent", tempDependent);

        //***********create device
        Relay *temp = new RelayMCP(param_value, convertPin(tempPin), atoi(tempDependent));
        deviceDelegate.addDevice( temp );
        //***********

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

        static_cast<RelayMCP*>(temp)->setI2C(sda, scl);
    

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
        static_cast<RelayMCP*>(temp)->setInvert(atoi(param_value));

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

void saveRelayPCA(Device *device) {

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

        static_cast<RelayPCA*>(device)->setI2C(sda, scl);
        
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "dependent", param_value);
        device->setDependentDevice(atoi(param_value));
        
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "event", param_value);
        device->setEvent(param_value);

        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "invert", param_value);
        // must be 1 or 0
        static_cast<RelayPCA*>(device)->setInvert(atoi(param_value));
        
}

void saveRelayMCP(Device *device) {

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

        static_cast<RelayMCP*>(device)->setI2C(sda, scl);
        
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "dependent", param_value);
        device->setDependentDevice(atoi(param_value));
        
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "event", param_value);
        device->setEvent(param_value);

        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "invert", param_value);
        // must be 1 or 0
        static_cast<RelayMCP*>(device)->setInvert(atoi(param_value));
        
}


