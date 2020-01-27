// © 2noodles llc
// minh@2noodles.com

void testEmail(WiFiClient client) {
  
      char subject[55] = {'\0'};
      char message[100] = {'\0'};
      char email[55] = {'\0'};
      webParser.parseQuery(queryBuffer, "email", email);
      //set email if it's not empty
      if(email[0] != '\0') {  
        wwws.setToEmail(email);
      }
      
      webParser.parseQuery(queryBuffer, "subject", subject);
    
      webParser.parseQuery(queryBuffer, "message", message);
      
      wwws.email(subject, message);
      
      successAjax(client);

  
}


void testIp(WiFiClient client) {
      webParser.clearBuffer(param_value, queryMax);
      webParser.parseQuery(queryBuffer, "ipaddress", param_value);
      wwws.setNetworkAddress(param_value);
      
      webParser.clearBuffer(param_value, queryMax);
      webParser.parseQuery(queryBuffer, "iphost", param_value);
      wwws.setNetworkHost(param_value);
      
      char holder[55]={'\0'};
      if(wwws.getNetworkIp(holder)) {
      //ajax requires a response or will throw a connection lost
        client.println(F("HTTP/1.1 200 OK"));
        client.println(F("Content-Type: text/xml"));
        client.println();
        client.println(F("<?xml version = \"1.0\" ?>"));
        client.print(F("<feed>"));
        client.print(F("<response>success</response>"));
        client.print(F("<ip>"));
        client.print(holder);
        Serial.print(holder);
        Serial.println(strlen(holder));
        client.print(F("</ip>"));
        client.print(F("</feed>"));
        
      } else {
        errorAjax(client);
      }
}

void testNTP(WiFiClient client) {
      webParser.clearBuffer(param_value, queryMax);
      webParser.parseQuery(queryBuffer, "testntp", param_value);
      wwws.setNTPServer(param_value);
      
      webParser.clearBuffer(param_value, queryMax);
      webParser.parseQuery(queryBuffer, "timezone", param_value);
      wwws.setTimeZone(atoi(param_value));

      webParser.clearBuffer(param_value, queryMax);
      webParser.parseQuery(queryBuffer, "daylight", param_value);
      wwws.setDayLightSaving(atoi(param_value));
      
      
      time_t temp = wwws.testNTP();
      time_t thour = hour(temp);
      time_t tminute = minute(temp);
      time_t tsecond = second(temp);
      
      if(temp) {
      //ajax requires a response or will throw a connection lost
        client.println(F("HTTP/1.1 200 OK"));
        client.println(F("Content-Type: text/xml"));
        client.println();
        client.println(F("<?xml version = \"1.0\" ?>"));
        client.print(F("<feed>"));
        client.print(F("<response>success</response>"));
        client.print(F("<time>"));
        client.print(thour);
        client.print(F(":"));
        client.print(tminute);
        client.print(F(":"));
        client.print(tsecond);
        client.print(F("</time>"));
        client.print(F("</feed>"));
        
      } else {
        errorAjax(client);
      }
}


void testDNS(WiFiClient client) {
   webParser.clearBuffer(param_value, queryMax);
      webParser.parseQuery(queryBuffer, "ddnshost", param_value);
      wwws.setDdnsHost(param_value);
      
      webParser.clearBuffer(param_value, queryMax);
      webParser.parseQuery(queryBuffer, "ddnshostname", param_value);
      wwws.setDdnsHostName(param_value);
      
      webParser.clearBuffer(param_value, queryMax);
      webParser.parseQuery(queryBuffer, "user", param_value);
      wwws.setUser(param_value);
      
      webParser.clearBuffer(param_value, queryMax);
      webParser.parseQuery(queryBuffer, "password", param_value);
      wwws.setPassword(param_value);

      if(wwws.updateDDNS()) {
      //ajax requires a response or will throw a connection lost
       successAjax(client);
        
      } else {
        errorAjax(client);
      }
}

void saveSetting(WiFiClient client, Config config) {
      webParser.clearBuffer(param_value, queryMax);
      webParser.parseQuery(queryBuffer, "email", param_value);
      wwws.setToEmail(param_value);
      
      char thour[4] = {'\0'};
      char tminute[4] = {'\0'};
      webParser.parseQuery(queryBuffer, "hour", thour);
      webParser.parseQuery(queryBuffer, "minute", tminute);
      wwws.setEvent(atoi(thour),atoi(tminute));
      
      webParser.clearBuffer(param_value, queryMax);
      webParser.parseQuery(queryBuffer, "istype", param_value);
      
      if(atoi(param_value) == 1) {
        wwws.setEmailIpState(true);
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "ipaddress", param_value);
        wwws.setNetworkAddress(param_value);
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "iphost", param_value);
        wwws.setNetworkHost(param_value);
    } else if(atoi(param_value) == 2) {
        wwws.setDDNSIpState(true);
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "ddnshost", param_value);
        wwws.setDdnsHost(param_value);
        
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "ddnshostname", param_value);
        wwws.setDdnsHostName(param_value);
        
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "user", param_value);
        wwws.setUser(param_value);
        
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "password", param_value);
        wwws.setPassword(param_value);
    }
      
    webParser.clearBuffer(param_value, queryMax);
    webParser.parseQuery(queryBuffer, "ntp", param_value);
    wwws.setNTPServer(param_value);
    
    strlcpy(param_value,              // <- destination
      config.ntpServer ,              // <- source
      sizeof(param_value));     // <- destination's capacity
    
    webParser.clearBuffer(param_value, queryMax);
    webParser.parseQuery(queryBuffer, "timezone", param_value);
    wwws.setTimeZone(atoi(param_value));
    
    config.timeZone = wwws.getTimeZone();

    saveConfiguration(configFile, config);
      
    //ajax requires a response or will throw a connection lost
    successAjax(client);
}

void getSetting(WiFiClient client) {
       client.println(F("HTTP/1.1 200 OK"));
      client.println(F("Content-Type: text/xml"));
      client.println();
      client.println(F("<?xml version = \"1.0\" encoding=\"UTF-8\"?>"));
      client.print(F("<setting>"));   
      client.print(F("<email>"));
      client.print(wwws.getToEmail());
      client.print(F("</email>"));
      if(wwws.getEmailIpState()) {
        client.print(F("<isEmail>"));
        client.print("1");
        client.print(F("</isEmail>"));
        client.print(F("<ipAddress>"));
        client.print(wwws.getNetworkAddress());
        client.print(F("</ipAddress>"));
        client.print(F("<ipHost>"));
        client.print(wwws.getNetworkHost());
        client.print(F("</ipHost>"));
      
      } else if(wwws.getDDNSIpState()) {
        client.print(F("<isDDNS>"));
        client.print("1");
        client.print(F("</isDDNS>"));
        client.print(F("<ddnsHost>"));
        client.print(wwws.getDdnsHost());
        client.print(F("</ddnsHost>"));
        client.print(F("<ddnsHostName>"));
        client.print(wwws.getDdnsHostName());
        client.print(F("</ddnsHostName>"));   
        client.print(F("<user>"));
        client.print(wwws.getUser());
        client.print(F("</user>"));
        client.print(F("<password>"));
        client.print(wwws.getPassword());
        client.print(F("</password>"));

      }
   
      client.print(F("<hour>"));
      client.print(wwws.getHour());
      client.print(F("</hour>"));
      
      client.print(F("<minute>"));
      client.print(wwws.getMinute());
      client.print(F("</minute>"));
      
      client.print(F("<ntp>"));
      client.print(wwws.getNTPServer());
      client.print(F("</ntp>"));
      
      client.print(F("<timezone>"));
      client.print(wwws.getTimeZone());
      client.print(F("</timezone>"));

      client.print(F("<daylight>"));
      client.print(wwws.getDayLightSaving());
      client.print(F("</daylight>"));
      
      client.print(F("</setting>"));
}
