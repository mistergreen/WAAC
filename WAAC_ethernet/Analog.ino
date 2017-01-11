void analogAjaxOutput(EthernetClient client, Device *device) {
       client.print(F("<pin>"));
       int tempInt = static_cast<Analog*>(device)->getPin();
       char tempChar[4];
       getAnalogCharAlias(tempInt, tempChar);
       client.print(tempChar);
       client.print(F("</pin>"));
       client.print(F("<min>"));
       client.print(static_cast<Analog*>(device)->getMin());
       client.print(F("</min>"));
       client.print(F("<max>"));
       client.print(static_cast<Analog*>(device)->getMax());
       client.print(F("</max>"));
       client.print(F("<lower>"));
       client.print(static_cast<Analog*>(device)->getLowerTarget());
       client.print(F("</lower>"));
       client.print(F("<higher>"));
       client.print(static_cast<Analog*>(device)->getHigherTarget());
       client.print(F("</higher>"));
       client.print(F("<unit>"));
       client.print(static_cast<Analog*>(device)->getUnit());
       client.print(F("</unit>"));
       client.print(F("<readout>"));
       client.print(static_cast<Analog*>(device)->read());
       client.print(F("</readout>"));
       client.print(F("<conditiontype>"));
       client.print(static_cast<Analog*>(device)->getConditionType());
       client.print(F("</conditiontype>"));
       client.print(F("<formula>"));
       client.print(static_cast<Analog*>(device)->getFormula());
       client.print(F("</formula>"));
}

void saveAnalog(Device *device) {
   
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "min", param_value);
        static_cast<Analog*>(device)->setMin(atof(param_value));
        
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "max", param_value);
        static_cast<Analog*>(device)->setMax(atof(param_value));
        
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "lower", param_value);
        static_cast<Analog*>(device)->setLowerTarget(atof(param_value));
        
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "higher", param_value);
        static_cast<Analog*>(device)->setHigherTarget(atof(param_value));
        
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "unit", param_value);
        static_cast<Analog*>(device)->setUnit(param_value);
        
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "formula", param_value);
      
        static_cast<Analog*>(device)->setFormula(param_value);
        
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "conditiontype", param_value);
        static_cast<Analog*>(device)->setConditionType(param_value);
            
  
}

int getAnalogIntAlias(char *pin) {
  int outpin;
  
  if(webParser.compare(pin, "A0") || webParser.compare(pin, "a0")) {
    outpin = 14;
    return outpin;
  } else if(webParser.compare(pin, "A1") || webParser.compare(pin, "a1")) {
    outpin = 15;
    return outpin;
  } else if(webParser.compare(pin, "A2") || webParser.compare(pin, "a2")) {
    outpin = 16;
    return outpin;
  } else if(webParser.compare(pin, "A3") || webParser.compare(pin, "a3")) {
    outpin = 17;
    return outpin;
  } else if(webParser.compare(pin, "A4") || webParser.compare(pin, "a4")) {
    outpin = 18;
    return outpin;
  } else if(webParser.compare(pin, "A5") || webParser.compare(pin, "a5")) {
    outpin = 19;
    return outpin;
  } 
  
  return atoi(pin); // in cast a number was entered
  
}

void getAnalogCharAlias(int pin, char *inArray) {

  if(pin == 14) {
    strcpy(inArray, "A0");
  } else if(pin == 15) {
    strcpy(inArray, "A1");
  } else if(pin == 16) {
    strcpy(inArray, "A2");
  } else if(pin == 17) {
    strcpy(inArray, "A3");
  } else if(pin == 18) {
    strcpy(inArray, "A4");
  } else if(pin == 19) {
    strcpy(inArray, "A5");
  } else {
    //strcpy(inArray, itoa(pin),10);
    sprintf(inArray,"%s", pin); // convert int to char
  }

}

void createAnalog() {
   //new Analog("Temperature Analog", A0)
         
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "name", param_value);
    
        char tempPin[5];
        webParser.parseQuery(queryBuffer, "pin", tempPin);
        
        
        //***********create device
        Analog *temp = new Analog(param_value, getAnalogIntAlias(tempPin));
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
        webParser.parseQuery(queryBuffer, "min", param_value);
        static_cast<Analog*>(temp)->setMin(atof(param_value));
        
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "max", param_value);
        static_cast<Analog*>(temp)->setMax(atof(param_value));
        
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "lower", param_value);
        static_cast<Analog*>(temp)->setLowerTarget(atof(param_value));
        
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "higher", param_value);
        static_cast<Analog*>(temp)->setHigherTarget(atof(param_value));
        
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "unit", param_value);
        static_cast<Analog*>(temp)->setUnit(param_value);
        
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "formula", param_value);
      
        static_cast<Analog*>(temp)->setFormula(param_value);
        
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "conditiontype", param_value);
        static_cast<Analog*>(temp)->setConditionType(param_value);
            
  
}


