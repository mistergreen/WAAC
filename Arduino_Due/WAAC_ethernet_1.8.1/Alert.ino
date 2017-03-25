void alertAjaxOutput(EthernetClient client, Device *device) {
      client.print(F("<dependent>"));
      client.print(device->getDependentDevice());
      client.print(F("</dependent>"));
      client.print(F("<email>"));
      client.print(static_cast<Alert*>(device)->getEmail());
      client.print(F("</email>"));
      client.print(F("<subject>"));
      client.print(static_cast<Alert*>(device)->getSubject());
      client.print(F("</subject>"));
      client.print(F("<message>"));
      client.print(static_cast<Alert*>(device)->getMessage());
      client.print(F("</message>"));
      client.print(F("<event>"));
      char string[7];
      static_cast<Alert*>(device)->getEvent(string);
      client.print(string);
      client.print(F("</event>"));
}


void saveAlert(Device *device) {
        //webParser.clearBuffer(param_value, queryMax);
        //webParser.parseQuery(queryBuffer, "email", param_value);
        static_cast<Alert*>(device)->setEmail(wwws.getToEmail());
       
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "subject", param_value);
        static_cast<Alert*>(device)->setSubject(param_value);
        
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "message", param_value);
        static_cast<Alert*>(device)->setMessage(param_value);
        
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "dependent", param_value);
        device->setDependentDevice(atoi(param_value));
        
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "event", param_value);
        device->setEvent(param_value);
        
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "state", param_value);
        device->setDeviceState(atoi(param_value));
  
}

void createAlert() {
  
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "name", param_value);
        
        //char email[45];
        //webParser.parseQuery(queryBuffer, "email", email);
   
        
        char tempDependent[5];
        webParser.parseQuery(queryBuffer, "dependent", tempDependent);
        
        //***********create device
        Alert *temp = new Alert(param_value, atoi(tempDependent));
        deviceDelegate.addDevice( temp );
        //***********
        
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "image", param_value);
        deviceDelegate.currentDevice()->setImageName(param_value);
        
        //
        char color[7];
        webParser.parseQuery(queryBuffer, "color", param_value);
        findColor(param_value, color);
        deviceDelegate.currentDevice()->setDeviceColor(color);
        
        //
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "event", param_value);
        if(webParser.compare(param_value, '\0')) {
          //Serial.println("event is a null xxxxxxxxxx");
        } else {
          deviceDelegate.currentDevice()->setEvent(param_value);
        }
        
        //
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "state", param_value);
        bool b;
        if(webParser.compare(param_value, "0")) 
            b = false;
        else 
            b = true;
            
        deviceDelegate.currentDevice()->setDeviceState(b);
        
        //
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "subject", param_value);
        static_cast<Alert*>(temp)->setSubject(param_value);
        
        //
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "message", param_value);
        static_cast<Alert*>(temp)->setMessage(param_value);
        
        static_cast<Alert*>(temp)->setEmail(wwws.getToEmail());

}

