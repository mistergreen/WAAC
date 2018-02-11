// © 2noodles llc
// minh@2noodles.com

void hallAjaxOutput(WiFiClient client, Device *device) {
       client.print(F("<pin>"));
       int tempInt = static_cast<HallSensor*>(device)->getPin();
       client.print(tempInt);
       client.print(F("</pin>"));
       client.print(F("<min>"));
       client.print(static_cast<HallSensor*>(device)->getMin());
       client.print(F("</min>"));
       client.print(F("<max>"));
       client.print(static_cast<HallSensor*>(device)->getMax());
       client.print(F("</max>"));
       client.print(F("<lower>"));
       client.print(static_cast<HallSensor*>(device)->getLowerTarget());
       client.print(F("</lower>"));
       client.print(F("<higher>"));
       client.print(static_cast<HallSensor*>(device)->getHigherTarget());
       client.print(F("</higher>"));
       client.print(F("<unit>"));
       client.print(static_cast<HallSensor*>(device)->getUnit());
       client.print(F("</unit>"));
       client.print(F("<readout>"));
       client.print(static_cast<HallSensor*>(device)->read());
       client.print(F("</readout>"));
       client.print(F("<conditiontype>"));
       client.print(static_cast<HallSensor*>(device)->getConditionType());
       client.print(F("</conditiontype>"));
       client.print(F("<formula>"));
       client.print(static_cast<HallSensor*>(device)->getFormula());
       client.print(F("</formula>"));
}

void saveHallSensor(Device *device) {
   
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "min", param_value);
        static_cast<HallSensor*>(device)->setMin(atof(param_value));
        
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "max", param_value);
        static_cast<HallSensor*>(device)->setMax(atof(param_value));
        
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "lower", param_value);
        static_cast<HallSensor*>(device)->setLowerTarget(atof(param_value));
        
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "higher", param_value);
        static_cast<HallSensor*>(device)->setHigherTarget(atof(param_value));
        
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "unit", param_value);
        static_cast<HallSensor*>(device)->setUnit(param_value);
        
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "formula", param_value);
      
        static_cast<HallSensor*>(device)->setFormula(param_value);
        
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "conditiontype", param_value);
        static_cast<HallSensor*>(device)->setConditionType(param_value);
            
  
}

void createHallSensor() {
         
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "name", param_value);
    
        char tempPin[5];
        webParser.parseQuery(queryBuffer, "pin", tempPin);
        
        
        //***********create device
        HallSensor *temp = new HallSensor(param_value, convertPin(tempPin));
        deviceDelegate.addDevice( temp );
        //***********
        //setup interrupt 
        // anonymous func []() {}
        static_cast<HallSensor*>(deviceDelegate.currentDevice())->setup([](){ static_cast<HallSensor*>(deviceDelegate.currentDevice())->rpm(); }, RISING);
        
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "image", param_value);
        deviceDelegate.currentDevice()->setImageName(param_value);
        
        char color[7];
        webParser.parseQuery(queryBuffer, "color", param_value);
        findColor(param_value, color);
        deviceDelegate.currentDevice()->setDeviceColor(color);
        
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "min", param_value);
        static_cast<HallSensor*>(temp)->setMin(atof(param_value));
        
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "max", param_value);
        static_cast<HallSensor*>(temp)->setMax(atof(param_value));
        
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "lower", param_value);
        static_cast<HallSensor*>(temp)->setLowerTarget(atof(param_value));
        
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "higher", param_value);
        static_cast<HallSensor*>(temp)->setHigherTarget(atof(param_value));
        
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "unit", param_value);
        static_cast<HallSensor*>(temp)->setUnit(param_value);
        
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "formula", param_value);
      
        static_cast<HallSensor*>(temp)->setFormula(param_value);
        
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "conditiontype", param_value);
        static_cast<HallSensor*>(temp)->setConditionType(param_value);
            
  
}


