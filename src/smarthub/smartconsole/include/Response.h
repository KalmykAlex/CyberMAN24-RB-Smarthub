#pragma once
#include "utils.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <cstdlib>
class Response
{
private:

    unsigned int code;
    unsigned int message_len;
    char* message = NULL;

public:
    Response(){
	
}
    Response(const char* message, unsigned int code);
    void clear();
    void ResponseMessage(char **);    
    ~Response();
    unsigned int getCode();
    unsigned int getMessageLen();
    char* getMsg();
};
