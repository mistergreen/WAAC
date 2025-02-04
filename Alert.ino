// © 2noodles llc
// minh@2noodles.com

void alertAjaxOutput(NetworkClient client, Device *device) {
      client.print(F("<dependent>"));
      client.print(static_cast<Alert*>(device)->getDependentDevice());
      client.print(F("</dependent>"));
      //use WWWsetting email
      //client.print(F("<email>"));
      //client.print(static_cast<Alert*>(device)->getEmail());
      //client.print(F("</email>"));
      client.print(F("<subject>"));
      client.print(static_cast<Alert*>(device)->getSubject());
      client.print(F("</subject>"));
      client.print(F("<message>"));
      client.print(static_cast<Alert*>(device)->getMessage());
      client.print(F("</message>"));
       client.print(F("<pin>"));
      client.print(static_cast<Alert*>(device)->getPin());
      client.print(F("</pin>"));
      client.print(F("<event>"));
      char string[7];
      static_cast<Alert*>(device)->getEvent(string);
      client.print(string);
      client.print(F("</event>"));
}

void createAlert() {
  

        //***********create device
        Alert *temp = new Alert();
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
