// Andrea Villa
// villa.andrea@gmail.com


void stepperAjaxOutput(NetworkClient client, Device *device) {
      client.print(F("<pin0>"));
      client.print(static_cast<StepperMotor*>(device)->getPin(0));
      client.print(F("</pin0>"));
      client.print(F("<pin1>"));
      client.print(static_cast<StepperMotor*>(device)->getPin(1));
      client.print(F("</pin1>"));
      client.print(F("<pin2>"));
      client.print(static_cast<StepperMotor*>(device)->getPin(2));
      client.print(F("</pin2>"));
      client.print(F("<pin3>"));
      client.print(static_cast<StepperMotor*>(device)->getPin(3));
      client.print(F("</pin3>"));
      client.print(F("<steps>"));
      client.print(static_cast<StepperMotor*>(device)->getSteps());
      client.print(F("</steps>"));
      client.print(F("<speed>"));
      client.print(static_cast<StepperMotor*>(device)->getSpeed());
      client.print(F("</speed>"));
      client.print(F("<totalSteps>"));
      client.print(static_cast<StepperMotor*>(device)->getTotalSteps());
      client.print(F("</totalSteps>"));
           
      client.print(F("<dependent>"));
      client.print(static_cast<StepperMotor*>(device)->getDependentDevice());
      client.print(F("</dependent>"));
      client.print(F("<event>"));
      char eventString[queryMax] = {'\0'};
      static_cast<StepperMotor*>(device)->getEvent(eventString);
      client.print(eventString);
      client.print(F("</event>"));
}

void createStepperMotor() {


        //***********create device
        StepperMotor *temp = new StepperMotor();
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
