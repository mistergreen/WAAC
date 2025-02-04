// © 2noodles llc
// minh@2noodles.com

void switchDigital(NetworkClient client) {
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


void switchMode(NetworkClient client) {
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


void relayAjaxOutput(NetworkClient client, Device *device) {

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

void inputAjaxOutput(NetworkClient client, Device *device) {
      client.print(F("<pin>"));
      client.print(device->getPin());
      client.print(F("</pin>"));
     
}


void inputMCPajaxOutput(NetworkClient client, Device *device) {
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

void inputButtonAjaxOutput(NetworkClient client, Device *device) {
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

void relayMCPajaxOutput(NetworkClient client, Device *device) {

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

void relayPCAajaxOutput(NetworkClient client, Device *device) {
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
        
        //***********create device
        Input *temp = new Input();
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

void createInputMCP() {

        
        //***********create device
        InputMCP *temp = new InputMCP();
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


void createInputButton() {

        
        //***********create device
        InputButton *temp = new InputButton();
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


void createRelay() {

        //***********create device
        Relay *temp = new Relay();
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

void createRelayPCA() {

        //***********create device
        RelayPCA *temp = new RelayPCA();
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

void createRelayMCP() {

        //***********create device /// crash on creating pin '6' - ESP32 pin 6 is set? causes crash
        RelayMCP *temp = new RelayMCP();
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
