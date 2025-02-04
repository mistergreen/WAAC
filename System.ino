// © 2noodles llc
// minh@2noodles.com

void testEmail(NetworkClient client) {
  
      char subject[55] = {'\0'};
      char message[100] = {'\0'};
      char email[55] = {'\0'};
      webParser.parseQuery(queryBuffer, "email", email);
      //set email if it's not empty
      if(email[0] != '\0') {  
        wwws->setToEmail(email);
      }
      
      webParser.parseQuery(queryBuffer, "subject", subject);
    
      webParser.parseQuery(queryBuffer, "message", message);
      
      wwws->email(subject, message);
      
      successAjax(client);
}


void testIp(NetworkClient client) {
      webParser.clearBuffer(param_value, queryMax);
      webParser.parseQuery(queryBuffer, "ipaddress", param_value);
      wwws->setNetworkAddress(param_value);
      
      webParser.clearBuffer(param_value, queryMax);
      webParser.parseQuery(queryBuffer, "iphost", param_value);
      wwws->setNetworkHost(param_value);
      
      char holder[55]={'\0'};
      if(wwws->getNetworkIp(holder)) {
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

void testNTP(NetworkClient client) {
      webParser.clearBuffer(param_value, queryMax);
      webParser.parseQuery(queryBuffer, "testntp", param_value);
      wwws->setNTPServer(param_value);
      
      webParser.clearBuffer(param_value, queryMax);
      webParser.parseQuery(queryBuffer, "timezone", param_value);
      wwws->setTimeZone(param_value);

      webParser.clearBuffer(param_value, queryMax);
      webParser.parseQuery(queryBuffer, "daylight", param_value);
      wwws->setDayLightSaving(atoi(param_value));
      
      Timezone* currentTime = wwws->getTime();
    
      if(currentTime) {
        time_t thour = currentTime->hour();
        time_t tminute = currentTime->minute();
        time_t tsecond = currentTime->second();
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


void testDNS(NetworkClient client) {
   webParser.clearBuffer(param_value, queryMax);
      webParser.parseQuery(queryBuffer, "ddnshost", param_value);
      wwws->setDdnsHost(param_value);
      
      webParser.clearBuffer(param_value, queryMax);
      webParser.parseQuery(queryBuffer, "ddnshostname", param_value);
      wwws->setDdnsHostName(param_value);
      
      webParser.clearBuffer(param_value, queryMax);
      webParser.parseQuery(queryBuffer, "user", param_value);
      wwws->setUser(param_value);
      
      webParser.clearBuffer(param_value, queryMax);
      webParser.parseQuery(queryBuffer, "password", param_value);
      wwws->setPassword(param_value);

      if(wwws->updateDDNS()) {
      //ajax requires a response or will throw a connection lost
       successAjax(client);
        
      } else {
        errorAjax(client);
      }
}

void saveSetting(NetworkClient client, Config config) {
      webParser.clearBuffer(param_value, queryMax);
      webParser.parseQuery(queryBuffer, "email", param_value);
      wwws->setToEmail(param_value);
      
      char thour[4] = {'\0'};
      char tminute[4] = {'\0'};
      webParser.parseQuery(queryBuffer, "hour", thour);
      webParser.parseQuery(queryBuffer, "minute", tminute);
      wwws->setEvent(atoi(thour),atoi(tminute));
      
      webParser.clearBuffer(param_value, queryMax);
      webParser.parseQuery(queryBuffer, "istype", param_value);
      
      if(atoi(param_value) == 1) {
        wwws->setEmailIpState(true);
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "ipaddress", param_value);
        wwws->setNetworkAddress(param_value);
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "iphost", param_value);
        wwws->setNetworkHost(param_value);
    } else if(atoi(param_value) == 2) {
        wwws->setDDNSIpState(true);
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "ddnshost", param_value);
        wwws->setDdnsHost(param_value);
        
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "ddnshostname", param_value);
        wwws->setDdnsHostName(param_value);
        
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "user", param_value);
        wwws->setUser(param_value);
        
        webParser.clearBuffer(param_value, queryMax);
        webParser.parseQuery(queryBuffer, "password", param_value);
        wwws->setPassword(param_value);
    }
      
    webParser.clearBuffer(param_value, queryMax);
    webParser.parseQuery(queryBuffer, "ntp", param_value);
    wwws->setNTPServer(param_value);

    webParser.clearBuffer(param_value, queryMax);
    webParser.parseQuery(queryBuffer, "timezone", param_value);
    wwws->setTimeZone(param_value);
    
    saveConfiguration(configFile);
      
    //ajax requires a response or will throw a connection lost
    successAjax(client);
}

void getSetting(NetworkClient client) {
      client.println(F("HTTP/1.1 200 OK"));
      client.println(F("Content-Type: text/xml"));
      client.println();
      client.println(F("<?xml version = \"1.0\" encoding=\"UTF-8\"?>"));
      client.print(F("<setting>"));   
      client.print(F("<email>"));
      client.print(wwws->getToEmail());
      client.print(F("</email>"));
      if(wwws->getEmailIpState()) {
        client.print(F("<isEmail>"));
        client.print("1");
        client.print(F("</isEmail>"));
        client.print(F("<ipAddress>"));
        client.print(wwws->getNetworkAddress());
        client.print(F("</ipAddress>"));
        client.print(F("<ipHost>"));
        client.print(wwws->getNetworkHost());
        client.print(F("</ipHost>"));
      
      } else if(wwws->getDDNSIpState()) {
        client.print(F("<isDDNS>"));
        client.print("1");
        client.print(F("</isDDNS>"));
        client.print(F("<ddnsHost>"));
        client.print(wwws->getDdnsHost());
        client.print(F("</ddnsHost>"));
        client.print(F("<ddnsHostName>"));
        client.print(wwws->getDdnsHostName());
        client.print(F("</ddnsHostName>"));   
        client.print(F("<user>"));
        client.print(wwws->getUser());
        client.print(F("</user>"));
        client.print(F("<password>"));
        client.print(wwws->getPassword());
        client.print(F("</password>"));

      }

      Timezone* currentTime = wwws->getTime();
      
      client.print(F("<hour>"));
      client.print(wwws->getHour());
      client.print(F("</hour>"));
      
      client.print(F("<minute>"));
      client.print(wwws->getMinute());
      client.print(F("</minute>"));
      
      client.print(F("<ntp>"));
      client.print(wwws->getNTPServer());
      client.print(F("</ntp>"));
      
      client.print(F("<timezone>"));
      client.print(wwws->getTimeZone());
      client.print(F("</timezone>"));

      client.print(F("<daylight>"));
      client.print(wwws->getDayLightSaving());
      client.print(F("</daylight>"));


      client.print(F("<localIP>"));
      client.print(wwws->getLocalIP());
      client.print(F("</localIP>"));
     
      client.print(F("<localSubnetMask>"));
      client.print(wwws->getLocalSubMask());
      client.print(F("</localSubnetMask>"));

      client.print(F("<localGateway>"));
      client.print(wwws->getLocalGateway());
      client.print(F("</localGateway>"));

      client.print(F("<macAddress>"));
      client.print(wwws->getLocalMacAddress());
      client.print(F("</macAddress>"));

      client.print(F("</setting>"));
}


void urldecode(char* str)
{
  // Create two pointers that point to the start of the data
  char *leader = str;
  char *follower = leader;
  
  // While we're not at the end of the string (current character not NULL)
  while (*leader) {
      // Check to see if the current character is a %
      if (*leader == '%') {
  
          // Grab the next two characters and move leader forwards
          leader++;
          char high = *leader;
          leader++;
          char low = *leader;
  
          // Convert ASCII 0-9A-F to a value 0-15
          if (high > 0x39) high -= 7;
          high &= 0x0f;
  
          // Same again for the low byte:
          if (low > 0x39) low -= 7;
          low &= 0x0f;
  
          // Combine the two into a single byte and store in follower:
          *follower = (high << 4) | low;
      } else {
          // All other characters copy verbatim
          *follower = *leader;
      }
  
      // Move both pointers to the next character:
      leader++;
      follower++;
  }
  // Terminate the new string with a NULL character to trim it off
  *follower = 0;

/*    String encodedString="";
    char c;
    char code0;
    char code1;
    for (int i =0; i < str.length(); i++){
        c=str.charAt(i);
      if (c == '+'){
        encodedString+=' ';  
      }else if (c == '%') {
        i++;
        code0=str.charAt(i);
        i++;
        code1=str.charAt(i);
        c = (h2int(code0) << 4) | h2int(code1);
        encodedString+=c;
      } else{
        
        encodedString+=c;  
      }
      
      yield();
    }
    
   return encodedString;*/
}

String urlencode(String str)
{
    String encodedString="";
    char c;
    char code0;
    char code1;
    char code2;
    for (int i =0; i < str.length(); i++){
      c=str.charAt(i);
      if (c == ' '){
        encodedString+= '+';
      } else if (isalnum(c)){
        encodedString+=c;
      } else{
        code1=(c & 0xf)+'0';
        if ((c & 0xf) >9){
            code1=(c & 0xf) - 10 + 'A';
        }
        c=(c>>4)&0xf;
        code0=c+'0';
        if (c > 9){
            code0=c - 10 + 'A';
        }
        code2='\0';
        encodedString+='%';
        encodedString+=code0;
        encodedString+=code1;
        //encodedString+=code2;
      }
      yield();
    }
    return encodedString;
    
}

unsigned char h2int(char c)
{
    if (c >= '0' && c <='9'){
        return((unsigned char)c - '0');
    }
    if (c >= 'a' && c <='f'){
        return((unsigned char)c - 'a' + 10);
    }
    if (c >= 'A' && c <='F'){
        return((unsigned char)c - 'A' + 10);
    }
    return(0);
}
