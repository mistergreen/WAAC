/*
 // © Andrea Villa
// villa.andrea@gmail.com
*/

void setPWM(WiFiClient client) 
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

void lightManagerAjaxOutput(WiFiClient client, Device *device) {
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
  client.print(F("<sensorPin>"));
  client.print(static_cast<LightManager*>(device)->getSensorPin());
  client.print(F("</sensorPin>"));
}

void saveLightManager(Device *device) { 
  webParser.clearBuffer(param_value, queryMax);
  webParser.parseQuery(queryBuffer, "lightControlPin", param_value);
  int white;

  if(param_value[0] == '\0') {
    white = UNSET;
  } else {
    white = convertPin(param_value);
  }

  webParser.clearBuffer(param_value, queryMax);
  webParser.parseQuery(queryBuffer, "lightControlChannel", param_value);
  int whiteChannel;

  if(param_value[0] == '\0') {
    whiteChannel = UNSET;
  } else {
    whiteChannel = atoi(param_value);
  }

  static_cast<LightManager*>(device)->setLightControlPin(white, whiteChannel);

  webParser.clearBuffer(param_value, queryMax);
  webParser.parseQuery(queryBuffer, "dependent", param_value);
  static_cast<LightManager*>(device)->setDependentDevice(convertPin(param_value));

  webParser.clearBuffer(param_value, queryMax);
  webParser.parseQuery(queryBuffer, "event", param_value);
  static_cast<LightManager*>(device)->setEvent(param_value);

  webParser.clearBuffer(param_value, queryMax);
  webParser.parseQuery(queryBuffer, "eventDark", param_value);
  static_cast<LightManager*>(device)->setEventDark(param_value);

  webParser.clearBuffer(param_value, queryMax);
  webParser.parseQuery(queryBuffer, "eventPwms", param_value);
  static_cast<LightManager*>(device)->deserializePwms(param_value);

  webParser.clearBuffer(param_value, queryMax);
  webParser.parseQuery(queryBuffer, "eventDarkPwms", param_value);
  static_cast<LightManager*>(device)->deserializeDarkPwms(param_value);
}

void createLightManager() {
  webParser.clearBuffer(param_value, queryMax);
  webParser.parseQuery(queryBuffer, "name", param_value);

    char tempDependent[5];
  webParser.parseQuery(queryBuffer, "dependent", tempDependent);

  //***********create device
  LightManager *temp = new LightManager(param_value, atoi(tempDependent));
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
  webParser.parseQuery(queryBuffer, "lightControlPin", param_value);
  int white;

  if(param_value[0] == '\0') {
    white = UNSET;
  } else {
    white = convertPin(param_value);
  }

  webParser.clearBuffer(param_value, queryMax);
  webParser.parseQuery(queryBuffer, "lightControlChannel", param_value);
  int whiteChannel;

  if(param_value[0] == '\0') {
    whiteChannel = UNSET;
  } else {
    whiteChannel = atoi(param_value);
  }

  temp->setLightControlPin(white, whiteChannel);

  webParser.clearBuffer(param_value, queryMax);
  webParser.parseQuery(queryBuffer, "event", param_value);
  temp->setEvent(param_value);

  webParser.clearBuffer(param_value, queryMax);
  webParser.parseQuery(queryBuffer, "eventPwms", param_value);
  temp->deserializePwms(param_value);

  webParser.clearBuffer(param_value, queryMax);
  webParser.parseQuery(queryBuffer, "relayPin", param_value);

  int relayPin;

  if(param_value[0] == '\0') {
    relayPin = UNSET;
  } else {
    relayPin = convertPin(param_value);
  }

  webParser.clearBuffer(param_value, queryMax);
  webParser.parseQuery(queryBuffer, "sensorPin", param_value);
  int sensorPin;

  if(param_value[0] == '\0') {
    sensorPin = UNSET;
  } else {
    sensorPin = convertPin(param_value);
  }
}
