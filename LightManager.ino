/*
 // © Andrea Villa
// villa.andrea@gmail.com
*/

void setPWM(NetworkClient client) 
{
  //use to set brightness
  webParser.clearBuffer(param_value, queryMax);
  webParser.parseQuery(queryBuffer, "setpwm", param_value);
  //don't perform if it's not a device

  if(atoi(param_value) > 0) {
    Device *device = deviceDelegate.findDevice(atoi(param_value));

    char white[4] = {'\0'};    
    webParser.parseQuery(queryBuffer, "white", white);

    static_cast<LightManager*>(device)->setPWM(atoi(white));
    
    successAjax(client);
  }
}

void switchDarkMode(NetworkClient client) {
      webParser.clearBuffer(param_value, queryMax);
      webParser.parseQuery(queryBuffer, "switchDark", param_value);
      int deviceId = atoi(param_value);
      if(deviceId > 0) { 
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "dark", param_value);

        if(0 == strcmp(param_value, "dark")) {
          static_cast<LightManager*>(deviceDelegate.findDevice(deviceId))->switchToDarkMode();
        }
        else {
          static_cast<LightManager*>(deviceDelegate.findDevice(deviceId))->switchToNormalMode();
        }
        Serial.print("switchDarkMode: ");
        Serial.println(param_value);
        successAjax(client);
      }
}

void lightManagerAjaxOutput(NetworkClient client, Device *device) {
  client.print(F("<pin>"));
  client.print(device->getPin());
  client.print(F("</pin>"));
      
  client.print(F("<dependent>"));
  client.print(static_cast<LightManager*>(device)->getDependentDevice());
  client.print(F("</dependent>"));
  client.print(F("<event>"));
  char string[queryMax] = {'\0'};
  static_cast<LightManager*>(device)->getEvent(string);
  client.print(string);
  client.print(F("</event>"));
  client.print(F("<eventPwms>"));
  char pwmString[LightManager::sPWMS_BUFFER_SIZE] = {'\0'};
  static_cast<LightManager*>(device)->serializePwms(pwmString);
  client.print(pwmString);
  client.print(F("</eventPwms>"));
  client.print(F("<eventDark>"));
  char darkString[queryMax] = {'\0'};
  static_cast<LightManager*>(device)->getEventDark(darkString);
  client.print(darkString);
  client.print(F("</eventDark>"));
  client.print(F("<eventDarkPwms>"));
  char pwmDarkString[LightManager::sPWMS_BUFFER_SIZE] = {'\0'};
  static_cast<LightManager*>(device)->serializeDarkPwms(pwmDarkString);
  client.print(pwmDarkString);
  client.print(F("</eventDarkPwms>"));
  int pin, channel;
  static_cast<LightManager*>(device)->getLightControlPin(&pin, &channel);
  client.print(F("<lightControlPin>"));
  client.print(pin);
  client.print(F("</lightControlPin>"));
  client.print(F("<lightControlChannel>"));
  client.print(channel);
  client.print(F("</lightControlChannel>"));
  client.print(F("<relayPin>"));
  client.print(static_cast<LightManager*>(device)->getRelayPin());
  client.print(F("</relayPin>"));
  client.print(F("<invert>"));
  client.print(static_cast<LightManager*>(device)->getInvert());
  client.print(F("</invert>"));
  client.print(F("<sensorPin>"));
  client.print(static_cast<LightManager*>(device)->getSensorPin());
  client.print(F("</sensorPin>"));
  client.print(F("<mode>"));
  client.print(static_cast<LightManager*>(device)->getSuspendTime());
  client.print(F("</mode>"));
  client.print(F("<dark>"));
  client.print(static_cast<LightManager*>(device)->getDarkModelMode());
  client.print(F("</dark>"));
}



void createLightManager() {
  

      //***********create device
      LightManager *temp = new LightManager();
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
