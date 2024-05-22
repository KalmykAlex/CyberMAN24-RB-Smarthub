#pragma once
#include <iostream>
#include <sqlite3.h>
#include "Logger.h"
#include "Room.h"
#include "utils.h"

class DBManager
{
    
private:

    std::string db_path = DB_PATH;
    std::string creationScript = CREATION_SCRIPT_PATH;
    sqlite3* db;
    Logger* logger = nullptr;
    
    DBManager();
    ~DBManager();
    static DBManager* dbInstance;
    int create_database_config();
    bool check_db();
    bool check_tables();
public:

    /* Define public methods */
    DBManager(DBManager &other) = delete;
    void operator=(const DBManager& other) = delete;
    static DBManager* GetDBInstance();
    static void destroyInstance();

    /* Generic query */
    char*** query(const std::string& sql_string, int* rows);
    bool insert_statement(std::string insert_statement, char* error_msg);

    /* Map database information to objects */
    std::vector<Room*>MapRoomList();
    std::vector<Device*> MapDeviceList();

    /* Update device */
    bool update_device_field(char* device_name, char* new_value);

};

