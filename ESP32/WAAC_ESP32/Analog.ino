// © 2noodles llc
// minh@2noodles.com

void analogAjaxOutput(WiFiClient client, Device *device) {
       client.print(F("<pin>"));
       int tempInt = static_cast<Analog*>(device)->getPin();
       client.print(tempInt);
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

void createAnalog() {
   //new Analog("Temperature Analog", A0)
         
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "name", param_value);
    
        char tempPin[5];
        webParser.parseQuery(queryBuffer, "pin", tempPin);
        
        
        //***********create device
        Analog *temp = new Analog(param_value, convertPin(tempPin));
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


