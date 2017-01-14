/*
  querystring reader
*/
#ifndef WebParser_h
#define WebParser_h


#include <Arduino.h> // needed for basic C funcs

class WebParser
{
  public:
    WebParser();
    //const char * parseQuery(char *s, char *inparam, char *param_value);
    //const char * fileUrl(char *s, char *param_value);
    const char * parseQuery(char *s, const char *inparam, char *param_value);
    const char * fileUrl(char *s, char *param_value);

    char * urlDecode(char *s);

    void clearBuffer( char *inbuff, int inlength);
    void splitTime(char *inString, int *inArray);
    bool contains(char *inBuff, const char *inString);
    bool compare(const char *ins1, const char *ins2);
    unsigned char h2int(char c);
    void urldecode(char *urlbuf);
 
  
};

#endif