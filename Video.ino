// © 2noodles llc
// minh@2noodles.com

void videoAjaxOutput(WiFiClient client, Device *device) {
    client.print(F("<video>"));
    //char code[20];
    
    client.print(static_cast<Video*>(device)->getCode());
    client.print(F("</video>"));
  
}

//you-tube code
//iKioaTlfxc8   < bad

void createVideo() {

        //***********create device
        //init with code crashes 
        //Video *temp = new Video(param_value, tempCode);
        Video *temp = new Video();
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
