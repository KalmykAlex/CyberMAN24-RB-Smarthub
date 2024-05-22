#pragma once


#include <iostream>
#include <fstream>
#include <pthread.h>

#include "json.h"
#define DEFAULT_BUFFER_SIZE 8192


#define WELCOME_MESSAGE "\033[36m\
 .-------------------------------------------------------------------------. \n\
|                                                                           |\n\
|                 ██████  ███▄ ▄███▓ ▄▄▄       ██▀███  ▄▄▄█████▓            |\n\
|               ▒██    ▒ ▓██▒▀█▀ ██▒▒████▄    ▓██ ▒ ██▒▓  ██▒ ▓▒            |\n\
|               ░ ▓██▄   ▓██    ▓██░▒██  ▀█▄  ▓██ ░▄█ ▒▒ ▓██░ ▒░            |\n\
|                 ▒   ██▒▒██    ▒██ ░██▄▄▄▄██ ▒██▀▀█▄  ░ ▓██▓ ░             |\n\
|               ▒██████▒▒▒██▒   ░██▒ ▓█   ▓██▒░██▓ ▒██▒  ▒██▒ ░             |\n\
|               ▒ ▒▓▒ ▒ ░░ ▒░   ░  ░ ▒▒   ▓▒█░░ ▒▓ ░▒▓░  ▒ ░░               |\n\
|               ░ ░▒  ░ ░░  ░      ░  ▒   ▒▒ ░  ░▒ ░ ▒░    ░                |\n\
|               ░  ░  ░  ░      ░     ░   ▒     ░░   ░   ░                  |\n\
|                     ░         ░         ░  ░   ░                          |\n\
|                          ██░ ██  █    ██  ▄▄▄▄                            |\n\
|                          ▓██░ ██▒ ██  ▓██▒▓█████▄                         |\n\
|                          ▒██▀▀██░▓██  ▒██░▒██▒ ▄██                        |\n\
|                          ░▓█ ░██ ▓▓█  ░██░▒██░█▀                          |\n\
|                          ░▓█▒░██▓▒▒█████▓ ░▓█  ▀█▓                        |\n\
|                          ▒ ░░▒░▒░▒▓▒ ▒ ▒ ░▒▓███▀▒                         |\n\
|                          ▒ ░▒░ ░░░▒░ ░ ░ ▒░▒   ░                          |\n\
|                          ░  ░░ ░ ░░░ ░ ░  ░    ░                          |\n\
|                          ░  ░  ░   ░      ░                               |\n\
|                                                  ░                        |\n\
 '-------------------------------------------------------------------------'\n\
 .-------------------------------------------------------------------------.\n\
|                                  WELCOME!                                 |\n\
|                         This is the first iteration                       |\n\
|               of the SmartHub app for smart device management!            |\n\
|                                                                           |\n\
|                                  Authors:                                 |\n\
|                                   \u001b[32mAC&DS\033[36m                                   |\n\
 '-------------------------------------------------------------------------'\n\n"

#define WELCOME_MESSAGE_WIN ".-------------------------------------------------------------------------. \n\
|                                                                           |\n\
|                           No cool banner for you.                         |\n\
|                        Cool people are using linux.                       |\n\
|                                                                           |\n\
|                                :P Hehehe...                               |\n\
|                                                                           |\n\
 '-------------------------------------------------------------------------'\n\
 .-------------------------------------------------------------------------.\n\
|                                  WELCOME!                                 |\n\
|                         This is the first iteration                       |\n\
|               of the SmartHub app for smart device management!            |\n\
|                                                                           |\n\
|                                  Authors:                                 |\n\
|                                   AC&DS                                   |\n\
 '-------------------------------------------------------------------------'\n\n"

#define STATUS_OK 200
#define OPERATION_FAILED 666
#define IP4_SIZE 32
#define DEVICE_TABLE_NO_OF_COLUMNS 4

#define DB_PATH "/cyberman24/data/smarthub/database/dvsa.db"
#define CREATION_SCRIPT_PATH "/cyberman24/services/smarthub/smartconsole/scripts/db_creation.sql"
#define GLOBAL_CONFIG_JSON_PATH "/cyberman24/data/smarthub/devices_config/device_types.json"
#define CONFIG_DIR_PATH "/cyberman24/data/smarthub/devices_config/"
#define LOG_PATH "/var/log/smarthub/"

std::string show_general_edit_options();
std::string show_edit_option_smart_bulb();
std::string show_edit_option_smart_lock();
Json::Value parse_json(const char* json_file_name);
void write_json_to_file(const Json::Value& json_value, std::string device_name);
 
enum class LogLevel { INFO, ERROR, WARNING };

extern pthread_mutex_t log_mutex;
