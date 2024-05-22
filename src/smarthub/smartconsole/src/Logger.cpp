#include "Logger.h"
#include <fstream>
#include <string>
#include <ctime>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <algorithm>

Logger* Logger::Instance = nullptr;

Logger::Logger() {
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%d_%m_%Y");
    log_file=oss.str()+".log";
}

Logger::~Logger(){
}

Logger *Logger::GetInstance() {
    if(Instance == nullptr) {
        Instance = new Logger();
    }
    
    return Instance;
}


void Logger::destroyInstance() {
    if(Instance != nullptr) {
        delete Instance;
        Instance = nullptr;
    }
}


 
void Logger::log(LogLevel level, std::string message) {
    // Get current date and time
    std::time_t now = std::time(nullptr);
    char timestamp[strlen("[dd-dm-YYYY HH:MM:ssz]")];
    std::strftime(timestamp, sizeof(timestamp), "[%d-%m-%Y %H:%M:%S]", std::localtime(&now));
 
    // Format log message
    std::string formatted_message = timestamp + std::string("[SmartHub]") + "[";
    switch (level) {
        case LogLevel::INFO:
            formatted_message += "INFO";
            break;
        case LogLevel::ERROR:
            formatted_message += "ERROR";
            break;
        case LogLevel::WARNING:
            formatted_message += "WARNING";
            break;
    }
    std::replace(message.begin(), message.end(), '\n', ' ');
    formatted_message += "][" + message+"]";
    
    std::string logs = log_path+'/'+log_file;
    
    pthread_mutex_lock(&log_mutex);
    std::ofstream logfile(logs, std::ios::app);
    if (logfile.is_open()) {
        logfile << formatted_message << std::endl;
        logfile.close();
    } else {
        std::cerr << "Error opening log file" << std::endl;
    }
    pthread_mutex_unlock(&log_mutex);
}

void Logger::log_error(const std::string& message){
    log(LogLevel::ERROR, message);
}

void Logger::log_info(const std::string& message){
    log(LogLevel::INFO, message);
}

void Logger::log_warning(const std::string& message){
    log(LogLevel::WARNING, message);
}