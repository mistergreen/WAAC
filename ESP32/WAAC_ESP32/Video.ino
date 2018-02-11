// © 2noodles llc
// minh@2noodles.com

void videoAjaxOutput(WiFiClient client, Device *device) {
    client.print(F("<video>"));
    //char code[20];
    
    client.print(static_cast<Video*>(device)->getCode());
    client.print(F("</video>"));
  
}

void saveAVideo(Device *device) {
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "code", param_value);
        
        static_cast<Video*>(device)->setCode(param_value);
}
//you-tube code
//iKioaTlfxc8   < bad

void createVideo() {

        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "name", param_value);

        //***********create device
        //init with code crashes 
        //Video *temp = new Video(param_value, tempCode);
        Video *temp = new Video(param_value);
        deviceDelegate.addDevice( temp );
        //***********

        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "image", param_value);
        deviceDelegate.currentDevice()->setImageName(param_value);
        
        char color[7];
        webParser.parseQuery(queryBuffer, "color", param_value);
        findColor(param_value, color);
        deviceDelegate.currentDevice()->setDeviceColor(color);

        char tempCode[20]={'\0'};
        webParser.parseQuery(queryBuffer, "code", tempCode);
        static_cast<Video*>(temp)->setCode(tempCode);

        
}

