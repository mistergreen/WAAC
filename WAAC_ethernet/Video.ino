void videoAjaxOutput(EthernetClient client, Device *device) {
    client.print(F("<video>"));
    client.print(static_cast<Video*>(device)->getCode());
    client.print(F("</video>"));
  
}

void saveAVideo(Device *device) {
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "code", param_value);
        
        static_cast<Video*>(device)->setCode(param_value);
}


void createVideo() {
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "name", param_value);
        
        char tempCode[13];
        webParser.parseQuery(queryBuffer, "code", tempCode);
        
        //***********create device
        Video *temp = new Video(param_value, tempCode);
        deviceDelegate.addDevice( temp );
        //***********
        
          
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "image", param_value);
        deviceDelegate.currentDevice()->setImageName(param_value);
        
        char color[7];
        webParser.parseQuery(queryBuffer, "color", param_value);
        findColor(param_value, color);
        deviceDelegate.currentDevice()->setDeviceColor(color);
        
}

