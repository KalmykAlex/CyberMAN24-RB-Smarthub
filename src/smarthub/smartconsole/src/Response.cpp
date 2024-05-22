#include "Response.h"

Response::Response(const char* message, unsigned int code)
{ 
    this->code = code;
    this->message_len = strlen(message) + 4 + 1;
    this->message = new char[this->message_len];
    memset(this->message, 0, this->message_len);
    memcpy(this->message, "[+] ", 4);
    memcpy(this->message + 4, message, strlen(message));
}

Response::~Response(){

}

unsigned int Response::getMessageLen(){
	return this->message_len;
}

void Response::clear(){

	if(this->message != NULL) {
		delete this->message;
		this->message = NULL;
	
	}

}

void Response::ResponseMessage(char **final_message)
{
    memcpy(*final_message, this->message, this->message_len);
   
}

unsigned int Response::getCode(){
    return this->code;
}

char* Response::getMsg(){
    return this->message;
}
