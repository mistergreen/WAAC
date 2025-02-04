// © 2noodles llc
// minh@2noodles.com
// Andrea Villa
// villa.andrea@gmail.com


void servoAjaxOutput(NetworkClient client, Device *device) {
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

 
        //***********create device
        ServoMotor *temp = new ServoMotor();
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
