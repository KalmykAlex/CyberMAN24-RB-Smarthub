#pragma once
#include <iostream>
#include <string.h>
#include "Response.h"
#include "User.h"
#include "DBManager.h"

static DBManager* db = DBManager::GetDBInstance();
static Logger* logger = Logger::GetInstance();

class ConsoleManagement
{ 

    static std::string get_device_specific_data(std::string device_name);
    static std::string format_string(const std::string& input);
    static int update_smart_bulb(Device* device, int option, int connfd);
    static int update_smart_lock(Device* device, int option, int connfd);
    static int update_air_quality(Device* device, int option, int connfd);

public:

    ConsoleManagement();
    static Response EvaluateCommand(const char* command, User& user);
    static Response EvaluateLogin(std::string username, std::string password);
    static Response EvaluateAnonCommand(char* command, User& user);
    static std::string take_input(int user_fd, const std::string& prompt=std::string(), const std::string& input_prompt=std::string());
};
