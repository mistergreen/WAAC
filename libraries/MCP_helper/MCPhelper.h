/*
 MCPhelper
// © 2noodles llc
// minh@2noodles.com
*/


#ifndef MCPhelper_h
#define MCPhelper_h


#include "Adafruit_MCP23017.h"

class MCPhelper
{
  public:

    static bool isSet;
    static void init();
    static Adafruit_MCP23017 mcp;
  private:
    MCPhelper() {} //disallow instance creation
};
#endif