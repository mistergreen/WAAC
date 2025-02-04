// © 2noodles llc
// minh@2noodles.com


#include "WebParser.h"

WebParser::WebParser()
{

}

//cleaner, more versatile
const char * WebParser::parseQuery(char *s, const char *inparam, char *param_value) {
    char *name_start_pos;
    char *value_start_pos;
    size_t name_length;
    size_t value_length;

    name_length = strlen(inparam);
    
    char *ptr = strstr(s,inparam);
    //get out if nothing
    if(ptr == NULL) return 0;
    name_start_pos = ptr;
    s = name_start_pos;
    s += strlen(inparam);
   
    //forget looking for '='. we know.
    value_start_pos = ++s;
    value_length = 0;
    // look for ampersand end-of-string or newline for value
    
    //GET
    while( *s != '&' && *s != ' ' && *s != '\0' && *s != '\n' && *s != '\r') {
        value_length++;
        s++;
    }

    if (value_length > 0) {
        strncpy(param_value, value_start_pos, value_length);
        
        return param_value;
    }

    return 0;
    
    
}

const char * WebParser::parsePost(char *s, const char *inparam, char *param_value) {
    char *name_start_pos;
    char *value_start_pos;
    size_t name_length;
    size_t value_length;

    name_length = strlen(inparam);
    
    char *ptr = strstr(s,inparam);
    //get out if nothing
    if(ptr == NULL) return 0;
    name_start_pos = ptr;
    s = name_start_pos;
    s += strlen(inparam);
   
    //forget looking for '='. we know.
    value_start_pos = ++s;
    value_length = 0;
    // look for ampersand end-of-string or newline for value
    // allow space in value
    //POST
    while( *s != '&' && *s != '\0' && *s != '\n' && *s != '\r') {
        value_length++;
        s++;
    }

    if (value_length > 0) {
        strncpy(param_value, value_start_pos, value_length);
        
        return param_value;
    }

    return 0;
    
   
    
}


const char * WebParser::fileUrl(char *s, char *param_value) {
    char *name_start_pos;
    //char *value_start_pos;
    size_t name_length;
   
    //find position of substring
    char *ptr = strchr(s,'/');
    int pos = ptr - s;
    
    for (int i=0; i < pos; i++) {
        s++;
    }
        
        name_start_pos = s;
        name_length = 0;

        // look for space or end-of-string or newline for value
        while( *s != ' ' && *s != '\0' && *s != '\n') {
            name_length++;
            s++;
        }
        
        if (name_length > 0) {
            strncpy(param_value, name_start_pos, name_length);
            return param_value;
        }
  
    //Serial.println("nothing");
    //return 0;
}

//found at https://github.com/robomotic/Hardware/blob/master/EtherCard/websrv_help_functions.cpp
// convert a single hex digit character to its integer value
unsigned char WebParser::h2int(char c)
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

// decode a url string e.g "hello%20joe" or "hello+joe" becomes "hello joe"
void WebParser::urldecode(char *urlbuf)
{
    char c;
    char *dst;
    dst=urlbuf;
    while ((c = *urlbuf)) {
        if (c == '+') c = ' ';
        if (c == '%') {
            urlbuf++;
            c = *urlbuf;
            urlbuf++;
            c = (h2int(c) << 4) | h2int(*urlbuf);
        }
        *dst = c;
        dst++;
        urlbuf++;
    }
    *dst = '\0';
}
//////////////

void WebParser::clearBuffer( char *inbuff , int inlength ) {
   //clear out param for new param
    memset(&inbuff[0], '\0', inlength);
    /*
    Serial.println(">>>>> size of buff: ");
    Serial.println(sizeof(inbuff));
    for (int i = 0; i < sizeof(inbuff); i++) {
        inbuff[i] = 0;
    }
    */

}

void WebParser::splitTime(char *inString, int *inArray) {
    char *tokens;
    int i = 0;
    tokens = strtok(inString, ",");
    while (tokens != NULL) {
        inArray[i] = atoi(tokens);
        tokens = strtok(NULL, ",");
        i++;
    }
    
    
}

bool WebParser::contains(char *inBuff, const char *inString) {
    
    if(strstr(inBuff, inString) == NULL) {
        return false;
    } else {
        return true;
    }
}

bool WebParser::compare(const char *ins1, const char *ins2) {
    
    if(strcmp(ins1, ins2) == 0) {
        //Serial.println("compare true");
        return true;
    } else {
        //Serial.println("compare false");
        return false;
    }
}



