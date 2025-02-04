/*
 MCPhelper
// © 2noodles llc
// minh@2noodles.com
*/

#include <Arduino.h>
#include "MCPhelper.h"

bool MCPhelper::isSet = false;
Adafruit_MCP23017 MCPhelper::mcp = Adafruit_MCP23017();

void MCPhelper::init() {
    
    //pin config
    // GPA0 = #21 alias 0
    // GPB0 = #1 alias 8
    // GPB1 = #2 alias 9
    // Looks like you can only begin() once or it'll blow up
    // maybe do it once in a static class
    mcp.begin();      // use default address 0
    isSet = true;
    Serial.println("MCP begin");
    
}

