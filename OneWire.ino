// © 2noodles llc
// minh@2noodles.com

void oneWireAjaxOutput(NetworkClient client, Device *device) {
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
     client.print(static_cast<OneWireSensor*>(device)->getUnit());
     client.print(F("</unit>"));
     client.print(F("<readout>"));
     client.print(static_cast<OneWireSensor*>(device)->read());
     client.print(F("</readout>"));
     client.print(F("<conditiontype>"));
     client.print(static_cast<OneWireSensor*>(device)->getConditionType());
     client.print(F("</conditiontype>"));
  
}


void createOneWire() {

        
      //***********create device
      OneWireSensor *temp = new OneWireSensor();
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
