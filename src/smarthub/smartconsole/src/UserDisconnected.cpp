#include "UserDisconnected.h"

UserDisconnected::UserDisconnected(){
    code=666;
    message=strdup("User has disconnected!");
}

UserDisconnected::~UserDisconnected(){
    code=-1;
}

UserDisconnected::UserDisconnected(const char* msg, int c){
    message=(char*)msg;
    code=c;
}

char* UserDisconnected::what(){
    return message;
}

int UserDisconnected::get_code(){
    return code;
}

char* UserDisconnected::get_message(){
    return message;
}