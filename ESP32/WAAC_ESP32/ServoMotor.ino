// © 2noodles llc
// minh@2noodles.com
// Andrea Villa
// villa.andrea@gmail.com


void servoAjaxOutput(WiFiClient client, Device *device) {
      client.print(F("<pin>"));
      client.print(device->getPin());
      client.print(F("</pin>"));
      client.print(F("<stopAngle>"));
      client.print(static_cast<ServoMotor*>(device)->getStopAngle());
      client.print(F("</stopAngle>"));
      client.print(F("<moveAngle>"));
      client.print(static_cast<ServoMotor*>(device)->getMoveAngle());
      client.print(F("</moveAngle>"));
      client.print(F("<duration>"));
      client.print(static_cast<ServoMotor*>(device)->getDuration());
      client.print(F("</duration>"));
           
      client.print(F("<dependent>"));
      client.print(static_cast<ServoMotor*>(device)->getDependentDevice());
      client.print(F("</dependent>"));
      client.print(F("<event>"));
      char eventString[queryMax] = {'\0'};
      static_cast<ServoMotor*>(device)->getEvent(eventString);
      client.print(eventString);
      client.print(F("</event>"));
}

void createServoMotor() {

        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "name", param_value);
        
        char tempPin[5]={'\0'};
        webParser.parseQuery(queryBuffer, "pin", tempPin);
        
        char tempDependent[5];
        webParser.parseQuery(queryBuffer, "dependent", tempDependent);

        char tempStopAngle[5]={'\0'};
        webParser.parseQuery(queryBuffer, "stopAngle", tempStopAngle);

        char tempMoveAngle[5]={'\0'};
        webParser.parseQuery(queryBuffer, "moveAngle", tempMoveAngle);

        char tempDuration[5]={'\0'};
        webParser.parseQuery(queryBuffer, "duration", tempDuration);
        
        Serial.print("create pin: ");
        Serial.println(tempPin);
        //***********create device
        ServoMotor *temp = new ServoMotor(param_value, convertPin(tempPin), atoi(tempDependent), atoi(tempStopAngle), atoi(tempMoveAngle), atoi(tempDuration));
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
}

void saveServoMotor(Device *device) {
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "dependent", param_value);
        static_cast<ServoMotor*>(device)->setDependentDevice(atoi(param_value));
        
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "event", param_value);
        static_cast<ServoMotor*>(device)->setEvent(param_value);

        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "duration", param_value);
        static_cast<ServoMotor*>(device)->setDuration(atoi(param_value));
   
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "stopAngle", param_value);
        static_cast<ServoMotor*>(device)->setStopAngle(atoi(param_value));

        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "moveAngle", param_value);
        static_cast<ServoMotor*>(device)->setMoveAngle(atoi(param_value));
}
