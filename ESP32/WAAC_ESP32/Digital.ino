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


void switchMode(WiFiClient client) {
      webParser.clearBuffer(param_value, queryMax);
      webParser.parseQuery(queryBuffer, "switchMode", param_value);
      int deviceId = atoi(param_value);
      if(deviceId > 0) { 
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "mode", param_value);

        if(0 == strcmp(param_value, "auto")) {
          deviceDelegate.findDevice(deviceId)->setSuspendTime(false);
        }
        else {
          deviceDelegate.findDevice(deviceId)->setSuspendTime(true);
        }
        Serial.print("switchMode: ");
        Serial.println(param_value);
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
      client.print(static_cast<Relay*>(device)->getDependentDevice());
      client.print(F("</dependent>"));
      client.print(F("<event>"));
      char eventString[queryMax] = {'\0'};
      static_cast<Relay*>(device)->getEvent(eventString);
      client.print(eventString);
      client.print(F("</event>"));
      client.print(F("<mode>"));
      client.print(static_cast<Relay*>(device)->getSuspendTime());
      client.print(F("</mode>"));

}

void inputAjaxOutput(WiFiClient client, Device *device) {
      client.print(F("<pin>"));
      client.print(device->getPin());
      client.print(F("</pin>"));
     
}


void inputMCPajaxOutput(WiFiClient client, Device *device) {
      client.print(F("<pin>"));
      client.print(device->getPin());
      client.print(F("</pin>"));
      
      int i2cArray[2];
      static_cast<InputMCP*>(device)->getI2C(i2cArray);
      client.print(F("<sda>"));
      client.print(i2cArray[0]);
      client.print(F("</sda>"));
      client.print(F("<scl>"));
      client.print(i2cArray[1]);
      client.print(F("</scl>"));
     
}

void inputButtonAjaxOutput(WiFiClient client, Device *device) {
      client.print(F("<pin>"));
      client.print(device->getPin());
      client.print(F("</pin>"));
      client.print(F("<deviceclick>"));
      client.print(static_cast<InputButton*>(device)->getClickDevice());
      client.print(F("</deviceclick>"));
      client.print(F("<actionclick>"));
      client.print(static_cast<InputButton*>(device)->getClickAction());
      client.print(F("</actionclick>"));
      client.print(F("<devicedoubleclick>"));
      client.print(static_cast<InputButton*>(device)->getDoubleClickDevice());
      client.print(F("</devicedoubleclick>"));
      client.print(F("<actiondoubleclick>"));
      client.print(static_cast<InputButton*>(device)->getDoubleClickAction());
      client.print(F("</actiondoubleclick>"));
      client.print(F("<devicetripleclick>"));
      client.print(static_cast<InputButton*>(device)->getTripleClickDevice());
      client.print(F("</devicetripleclick>"));
      client.print(F("<actiontripleclick>"));
      client.print(static_cast<InputButton*>(device)->getTripleClickAction());
      client.print(F("</actiontripleclick>"));
      client.print(F("<devicelongclick>"));
      client.print(static_cast<InputButton*>(device)->getLongClickDevice());
      client.print(F("</devicelongclick>"));
      client.print(F("<actionlongclick>"));
      client.print(static_cast<InputButton*>(device)->getLongClickAction());
      client.print(F("</actionlongclick>"));

      client.print(F("<system>"));
      client.print(F("<systemaction>"));
      client.print(F("<action_name>"));
      client.print(F("Reconnect"));
      client.print(F("</action_name>"));
      client.print(F("<action_id>"));
      client.print(0);
      client.print(F("</action_id>"));
      client.print(F("</systemaction>"));
      client.print(F("<systemaction>"));
      client.print(F("<action_name>"));
      client.print(F("Reset"));
      client.print(F("</action_name>"));
      client.print(F("<action_id>"));
      client.print(1);
      client.print(F("</action_id>"));
      client.print(F("</systemaction>"));
      client.print(F("</system>"));
}

void relayMCPajaxOutput(WiFiClient client, Device *device) {

      client.print(F("<pin>"));
      client.print(device->getPin());
      client.print(F("</pin>"));
      client.print(F("<invert>"));
      client.print(static_cast<RelayMCP*>(device)->getInvert());
      client.print(F("</invert>"));
      int i2cArray[2];
      static_cast<RelayMCP*>(device)->getI2C(i2cArray);
      client.print(F("<sda>"));
      client.print(i2cArray[0]);
      client.print(F("</sda>"));
      client.print(F("<scl>"));
      client.print(i2cArray[1]);
      client.print(F("</scl>"));
             
      client.print(F("<dependent>"));
      client.print(static_cast<Relay*>(device)->getDependentDevice());
      client.print(F("</dependent>"));
      client.print(F("<event>"));
      char eventString[queryMax] = {'\0'};
      static_cast<RelayMCP*>(device)->getEvent(eventString);
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
      client.print(static_cast<Relay*>(device)->getDependentDevice());
      client.print(F("</dependent>"));
      client.print(F("<event>"));
      char eventString[queryMax] = {'\0'};
      static_cast<RelayPCA*>(device)->getEvent(eventString);
      client.print(eventString);
      client.print(F("</event>"));
}

void createInput() {

        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "name", param_value);
        
        char tempPin[5]={'\0'};
        webParser.parseQuery(queryBuffer, "pin", tempPin);
        
        //***********create device
        Input *temp = new Input(param_value, convertPin(tempPin));
        deviceDelegate.addDevice( temp );
        //***********

        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "image", param_value);
        deviceDelegate.currentDevice()->setImageName(param_value);
        
        char color[7];
        webParser.parseQuery(queryBuffer, "color", param_value);
        findColor(param_value, color);
        deviceDelegate.currentDevice()->setDeviceColor(color);

}

void createInputMCP() {

        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "name", param_value);
        
        char tempPin[5]={'\0'};
        webParser.parseQuery(queryBuffer, "pin", tempPin);
        
        //***********create device
        InputMCP *temp = new InputMCP(param_value, convertPin(tempPin));
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

        static_cast<InputMCP*>(temp)->setI2C(sda, scl);
        

        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "image", param_value);
        deviceDelegate.currentDevice()->setImageName(param_value);
        
        char color[7];
        webParser.parseQuery(queryBuffer, "color", param_value);
        findColor(param_value, color);
        deviceDelegate.currentDevice()->setDeviceColor(color);

}


void createInputButton() {
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "name", param_value);
        
        char tempPin[5]={'\0'};
        webParser.parseQuery(queryBuffer, "pin", tempPin);
        
        //***********create device
        InputButton *temp = new InputButton(param_value, convertPin(tempPin));
        deviceDelegate.addDevice( temp );
        //***********

        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "image", param_value);
        deviceDelegate.currentDevice()->setImageName(param_value);
        
        char color[7];
        webParser.parseQuery(queryBuffer, "color", param_value);
        findColor(param_value, color);
        deviceDelegate.currentDevice()->setDeviceColor(color);

        char tempDevice[4] = {'\0'};
        char tempAction[4] = {'\0'};
        webParser.parseQuery(queryBuffer, "deviceClick", tempDevice);
        webParser.parseQuery(queryBuffer, "actionClick", tempAction);
        temp->setClickAction(atoi(tempDevice), atoi(tempAction)); 

        //clear out param for new param
        memset(&tempDevice[0], '\0', 4);
        memset(&tempAction[0], '\0', 4);
        webParser.parseQuery(queryBuffer, "deviceDoubleClick", tempDevice);
        webParser.parseQuery(queryBuffer, "actionDoubleClick", tempAction);
        temp->setDoubleClickAction(atoi(tempDevice), atoi(tempAction)); 

        //clear out param for new param
        memset(&tempDevice[0], '\0', 4);
        memset(&tempAction[0], '\0', 4);
        webParser.parseQuery(queryBuffer, "deviceTripleClick", tempDevice);
        webParser.parseQuery(queryBuffer, "actionTripleClick", tempAction);
        temp->setTripleClickAction(atoi(tempDevice), atoi(tempAction)); 

        //clear out param for new param
        memset(&tempDevice[0], '\0', 4);
        memset(&tempAction[0], '\0', 4);

        webParser.parseQuery(queryBuffer, "deviceLongClick", tempDevice);
        webParser.parseQuery(queryBuffer, "actionLongClick", tempAction);
        temp->setLongClickAction(atoi(tempDevice), atoi(tempAction));
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
        temp->setEvent(param_value);


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
        RelayPCA *temp = new RelayPCA(param_value, convertPin(tempPin), atoi(tempDependent));
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
        temp->setEvent(param_value);


        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "invert", param_value);
        // must be 1 or 0
        static_cast<RelayPCA*>(temp)->setInvert(atoi(param_value));

}

void createRelayMCP() {

        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "name", param_value);
        
        char tempPin[2] = {'\0'};
        webParser.parseQuery(queryBuffer, "pin", tempPin);
        
        char tempDependent[5];
        webParser.parseQuery(queryBuffer, "dependent", tempDependent);

        //***********create device /// crash on creating pin '6' - ESP32 pin 6 is set? causes crash
        RelayMCP *temp = new RelayMCP(param_value, atoi(tempPin), atoi(tempDependent));

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
        static_cast<RelayMCP*>(temp)->setEvent(param_value);


        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "invert", param_value);
        // must be 1 or 0
        static_cast<RelayMCP*>(temp)->setInvert(atoi(param_value));

}


void saveRelay(Device *device) {
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "dependent", param_value);
        static_cast<Relay*>(device)->setDependentDevice(atoi(param_value));
        
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "event", param_value);
        static_cast<Relay*>(device)->setEvent(param_value);

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
        static_cast<Relay*>(device)->setDependentDevice(atoi(param_value));
        
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "event", param_value);
        static_cast<Relay*>(device)->setEvent(param_value);

        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "invert", param_value);
        // must be 1 or 0
        static_cast<RelayPCA*>(device)->setInvert(atoi(param_value));
        
}

void saveInputMCP(Device *device) {
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

        static_cast<InputMCP*>(device)->setI2C(sda, scl);
}


void saveInputButton(Device *device) {
  char tempDevice[4] = {'\0'};
  char tempAction[4] = {'\0'};
  webParser.parseQuery(queryBuffer, "deviceClick", tempDevice);
  webParser.parseQuery(queryBuffer, "actionClick", tempAction);
  static_cast<InputButton*>(device)->setClickAction(atoi(tempDevice), atoi(tempAction)); 

  //clear out param for new param
  memset(&tempDevice[0], '\0', 4);
  memset(&tempAction[0], '\0', 4);
  webParser.parseQuery(queryBuffer, "deviceDoubleClick", tempDevice);
  webParser.parseQuery(queryBuffer, "actionDoubleClick", tempAction);
  static_cast<InputButton*>(device)->setDoubleClickAction(atoi(tempDevice), atoi(tempAction)); 

  //clear out param for new param
  memset(&tempDevice[0], '\0', 4);
  memset(&tempAction[0], '\0', 4);
  webParser.parseQuery(queryBuffer, "deviceTripleClick", tempDevice);
  webParser.parseQuery(queryBuffer, "actionTripleClick", tempAction);
  static_cast<InputButton*>(device)->setTripleClickAction(atoi(tempDevice), atoi(tempAction)); 

  //clear out param for new param
  memset(&tempDevice[0], '\0', 4);
  memset(&tempAction[0], '\0', 4);

  webParser.parseQuery(queryBuffer, "deviceLongClick", tempDevice);
  webParser.parseQuery(queryBuffer, "actionLongClick", tempAction);
  static_cast<InputButton*>(device)->setLongClickAction(atoi(tempDevice), atoi(tempAction));
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
        static_cast<Relay*>(device)->setDependentDevice(atoi(param_value));
        
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "event", param_value);
        static_cast<Relay*>(device)->setEvent(param_value);

        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "invert", param_value);
        // must be 1 or 0
        static_cast<RelayMCP*>(device)->setInvert(atoi(param_value));
        
}
