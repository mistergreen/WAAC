// © 2noodles llc
// minh@2noodles.com

void oneWireAjaxOutput(WiFiClient client, Device *device) {
      client.print(F("<pin>"));
      client.print(device->getPin());
      client.print(F("</pin>"));
            
     client.print(F("<min>"));
     client.print(static_cast<OneWireSensor*>(device)->getMin());
     client.print(F("</min>"));
     client.print(F("<max>"));
     client.print(static_cast<OneWireSensor*>(device)->getMax());
     client.print(F("</max>"));
     client.print(F("<lower>"));
     client.print(static_cast<OneWireSensor*>(device)->getLowerTarget());
     client.print(F("</lower>"));
     client.print(F("<higher>"));
     client.print(static_cast<OneWireSensor*>(device)->getHigherTarget());
     client.print(F("</higher>"));
     client.print(F("<unit>"));
     client.print(static_cast<OneWireSensor*>(device)->getF());
     client.print(F("</unit>"));
     client.print(F("<readout>"));
     client.print(static_cast<OneWireSensor*>(device)->read());
     client.print(F("</readout>"));
     client.print(F("<conditiontype>"));
     client.print(static_cast<OneWireSensor*>(device)->getConditionType());
     client.print(F("</conditiontype>"));
  
}

void saveOneWire(Device *device) {
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "min", param_value);
        static_cast<OneWireSensor*>(device)->setMin(atof(param_value));
        
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "max", param_value);
        static_cast<OneWireSensor*>(device)->setMax(atof(param_value));
        
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "lower", param_value);
        static_cast<OneWireSensor*>(device)->setLowerTarget(atof(param_value));
        
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "higher", param_value);
        static_cast<OneWireSensor*>(device)->setHigherTarget(atof(param_value));
        
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "unit", param_value);
        static_cast<OneWireSensor*>(device)->setF(atoi(param_value));
        
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "conditiontype", param_value);
        static_cast<OneWireSensor*>(device)->setConditionType(param_value);
}

void createOneWire() {
         webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "name", param_value);
        
        char tempPin[5];
        webParser.parseQuery(queryBuffer, "pin", tempPin);
        
        char minVal[10];
        webParser.parseQuery(queryBuffer, "min", minVal);
        
        char maxVal[10];
        webParser.parseQuery(queryBuffer, "max", maxVal);
        
        //***********create device
        OneWireSensor *temp = new OneWireSensor(param_value, convertPin(tempPin), atof(minVal), atof(maxVal));
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
        webParser.parseQuery(queryBuffer, "lower", param_value);
        static_cast<OneWireSensor*>(temp)->setLowerTarget(atof(param_value));
        
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "higher", param_value);
        static_cast<OneWireSensor*>(temp)->setHigherTarget(atof(param_value));
        
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "unit", param_value);
        static_cast<OneWireSensor*>(temp)->setF(atoi(param_value));
        
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "conditiontype", param_value);
        static_cast<OneWireSensor*>(temp)->setConditionType(param_value);
}

