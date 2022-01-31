// Andrea Villa
// villa.andrea@gmail.com


void stepperAjaxOutput(WiFiClient client, Device *device) {
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

        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "name", param_value);
        
        char tempPin0[5]={'\0'};
        webParser.parseQuery(queryBuffer, "pin0", tempPin0);

        char tempPin1[5]={'\0'};
        webParser.parseQuery(queryBuffer, "pin1", tempPin1);
        
        char tempPin2[5]={'\0'};
        webParser.parseQuery(queryBuffer, "pin2", tempPin2);
        
        char tempPin3[5]={'\0'};
        webParser.parseQuery(queryBuffer, "pin3", tempPin3);

        char tempDependent[5];
        webParser.parseQuery(queryBuffer, "dependent", tempDependent);
        
        char tempSteps[5];
        webParser.parseQuery(queryBuffer, "steps", tempSteps);

        char tempSpeed[5]={'\0'};
        webParser.parseQuery(queryBuffer, "speed", tempSpeed);

        char tempTotalSteps[5]={'\0'};
        webParser.parseQuery(queryBuffer, "totalSteps", tempTotalSteps);

        
        Serial.print("Create Stepper at pins: ");
        Serial.print(tempPin0);
        Serial.print("; ");
        Serial.print(tempPin1);
        Serial.print("; ");
        Serial.print(tempPin2);
        Serial.print("; ");
        Serial.println(tempPin3);
        //***********create device
        StepperMotor *temp = new StepperMotor(param_value, 
                                              convertPin(tempPin0), 
                                              convertPin(tempPin1), 
                                              convertPin(tempPin2), 
                                              convertPin(tempPin3),
                                              atoi(tempDependent), 
                                              atoi(tempTotalSteps), 
                                              atoi(tempSteps),
                                              atoi(tempSpeed));
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

void saveStepperMotor(Device *device) {
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "dependent", param_value);
        static_cast<StepperMotor*>(device)->setDependentDevice(atoi(param_value));
        
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "event", param_value);
        static_cast<StepperMotor*>(device)->setEvent(param_value);

        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "pin0", param_value);
        int pin0;
        pin0 = convertPin(param_value);

        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "pin1", param_value);
        int pin1;
        pin1 = convertPin(param_value);

        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "pin2", param_value);
        int pin2;
        pin2 = convertPin(param_value);

        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "pin3", param_value);
        int pin3;
        pin3 = convertPin(param_value);
        
        static_cast<StepperMotor*>(device)->setPins(pin0, pin1, pin2, pin3);
        
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "steps", param_value);
        static_cast<StepperMotor*>(device)->setSteps(atoi(param_value));
   
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "speed", param_value);
        static_cast<StepperMotor*>(device)->setSpeed(atoi(param_value));

        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "totalSteps", param_value);
        static_cast<StepperMotor*>(device)->setTotalSteps(atoi(param_value));
}
