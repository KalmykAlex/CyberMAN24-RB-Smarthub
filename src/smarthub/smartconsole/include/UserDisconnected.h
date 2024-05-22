#pragma once
#include <iostream>
#include <string.h>
#include <stdlib.h>

class UserDisconnected : public std::exception {
    char* message;
    int code;
public:
    UserDisconnected();
    ~UserDisconnected();
    UserDisconnected(const char* msg, int c);
    char* what();

    int get_code();
    char* get_message();
};