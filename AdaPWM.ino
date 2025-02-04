// © 2noodles llc
// minh@2noodles.com

void set8AdaPWMs(NetworkClient client) {
      webParser.clearBuffer(param_value, queryMax);
      webParser.parseQuery(queryBuffer, "set8adapwms", param_value);
      //don't perform if it's not a device
      if(atoi(param_value) > 0) {
        Device *device = deviceDelegate.findDevice(atoi(param_value));
        char red[5] = {'\0'};
        char green[5] = {'\0'};
        char blue[5] = {'\0'};
        char white[5] = {'\0'};
        char aux1[5] = {'\0'};
        char aux2[5] = {'\0'};
        char aux3[5] = {'\0'};
        char aux4[5] = {'\0'};
        
        webParser.parseQuery(queryBuffer, "red", red);
        
        webParser.parseQuery(queryBuffer, "green", green);
        
        webParser.parseQuery(queryBuffer, "blue", blue);
        
        webParser.parseQuery(queryBuffer, "white", white);
        
        webParser.parseQuery(queryBuffer, "aux1", aux1);
        
        webParser.parseQuery(queryBuffer, "aux2", aux2);
        
        webParser.parseQuery(queryBuffer, "aux3", aux3);
        
        webParser.parseQuery(queryBuffer, "aux4", aux4);
        
        
        static_cast<AdaFruitPWM8*>(device)->setPWMs(atoi(red),atoi(green),atoi(blue),atoi(white),atoi(aux1),atoi(aux2),atoi(aux3),atoi(aux4) );
        
        successAjax(client);
      }
}



void adaFruit8PWMAjaxOutput(NetworkClient client, Device *device) {
           client.print(F("<pin>"));
          client.print(device->getPin());
          client.print(F("</pin>"));
            
          client.print(F("<dependent>"));
          client.print(static_cast<AdaFruitPWM8*>(device)->getDependentDevice());
          client.print(F("</dependent>"));

          int i2cArray[2];
          static_cast<AdaFruitPWM8*>(device)->getI2C(i2cArray);
          client.print(F("<sda>"));
          client.print(i2cArray[0]);
          client.print(F("</sda>"));
          client.print(F("<scl>"));
          client.print(i2cArray[1]);
          client.print(F("</scl>"));
          // slider vals are in the events 
          client.print(F("<event>"));
          char eventstring[queryMax];
          static_cast<AdaFruitPWM8*>(device)->getEvent(eventstring);
//Serial.println(">>>> to html >>>>>>");
 //Serial.println(eventstring);
          client.print(eventstring);
          client.print(F("</event>"));
          client.print(F("<eventColors>"));
          char colorsString[queryMax] = {'\0'};
          static_cast<AdaFruitPWM8*>(device)->getEventColors(colorsString);
          client.print(colorsString);
         client.print(F("</eventColors>"));
          int pinArray[8];
          static_cast<AdaFruitPWM8*>(device)->getPins(pinArray);

          client.print(F("<redpin>"));
          client.print(pinArray[0]);
          client.print(F("</redpin>"));
          client.print(F("<greenpin>"));
          client.print(pinArray[1]);
          client.print(F("</greenpin>"));
          client.print(F("<bluepin>"));
          client.print(pinArray[2]);
          client.print(F("</bluepin>"));
          client.print(F("<whitepin>"));
          client.print(pinArray[3]);
          client.print(F("</whitepin>"));
         client.print(F("<aux1>"));
          client.print(pinArray[4]);
          client.print(F("</aux1>"));
          client.print(F("<aux2>"));
          client.print(pinArray[5]);
          client.print(F("</aux2>"));
          client.print(F("<aux3>"));
          client.print(pinArray[6]);
          client.print(F("</aux3>"));
          client.print(F("<aux4>"));
          client.print(pinArray[7]);
          client.print(F("</aux4>"));
       Serial.println("ada8 output");
}




//create ada8 - lock server if you dont create event?
void createAdaFruit8PWM() {

        
        //***********create device
        AdaFruitPWM8 *temp = new AdaFruitPWM8();
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
