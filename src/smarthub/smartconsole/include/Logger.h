#pragma once
#include <iostream>
#include "utils.h"

class Logger
{
    
private:

    std::string log_path = LOG_PATH;
    std::string log_file="default.logs";

    Logger();
    ~Logger();
    static Logger* Instance;

    void log(LogLevel level, std::string message);
public:

    /* Define public methods */
    Logger(Logger &other) = delete;
    void operator=(const Logger& other) = delete;
    static Logger* GetInstance();
    static void destroyInstance();

    void log_error(const std::string& message);
    void log_info(const std::string& message);
    void log_warning(const std::string& message);
};

