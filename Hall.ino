// © 2noodles llc
// minh@2noodles.com

void hallAjaxOutput(NetworkClient client, Device *device) {
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


void createHallSensor() {
         

        //***********create device
        HallSensor *temp = new HallSensor();
        deviceDelegate.addDevice( temp );
        //***********

        JsonObject obj = doc.as<JsonObject>();

        deviceDelegate.currentDevice()->deserialize(obj);

        char color[7];
        char imageName[30];
        strcpy(imageName, obj["imageName"]);
        findColor(imageName, color);
        deviceDelegate.currentDevice()->setDeviceColor(color);
        
         //setup interrupt - crashing ESP- Got to define pin first?
        // anonymous func []() {} --crashing because of the pin?
        static_cast<HallSensor*>(deviceDelegate.currentDevice())->setup([](){ static_cast<HallSensor*>(deviceDelegate.currentDevice())->rpm(); }, RISING);
        // crash test setup()
            
  
}
