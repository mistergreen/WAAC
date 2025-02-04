// © 2noodles llc
// minh@2noodles.com

void analogAjaxOutput(NetworkClient client, Device *device) {
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


void createAnalog() {

        
        //***********create device
        Analog *temp = new Analog();
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
