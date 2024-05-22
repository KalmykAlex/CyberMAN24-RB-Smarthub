#include "DBManager.h"
#include <fstream>
#include <string.h>
#include <sqlite3.h>
#include "Device.h"
#include "utils.h"
#include "SmartBulb.h"
#include "SmartLock.h"
#include "AirQualityMonitor.h"


DBManager* DBManager::dbInstance = nullptr;

DBManager::DBManager() {
    try
    {   
        logger = Logger::GetInstance();
        int rc = 0;
        int max_tries=3;
        do
        {
            rc = this->create_database_config();
            if(rc == SQLITE_OK){
                rc = sqlite3_open(DB_PATH, &db);

                if (rc != SQLITE_OK) {
                    std::cerr << "Error opening SQLite database: " << sqlite3_errmsg(db) << std::endl;
                    sqlite3_close(db);
                }
            }
            max_tries--;
            if(max_tries<0)
                throw std::runtime_error("DBManager error: Cannot create/open database!");
        } while (rc!=SQLITE_OK);
        logger->log_info("Database created from creation script.");
    }
    catch(const std::exception& e)
    {
        throw std::runtime_error("DBManager constructor error!");
        std::cerr << "DBManager error: " << e.what() << '\n';
    }
}

DBManager::~DBManager(){
    if(dbInstance!=nullptr)
        sqlite3_close(db);
}

bool DBManager::check_tables(){
    sqlite3 *temp;
    int rc = sqlite3_open(DB_PATH, &temp);
 
    if (rc != SQLITE_OK) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(temp) << std::endl;
        sqlite3_close(temp);
        return false;
    }
 
    // Query to check if tables exist
    const char *sql = "SELECT name FROM sqlite_master WHERE type='table' AND (name='users' OR name='devices' OR name='rooms')";
 
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(temp, sql, -1, &stmt, nullptr);
 
    if (rc != SQLITE_OK) {
        std::cerr << "Error preparing SQL statement: " << sqlite3_errmsg(temp) << std::endl;
        sqlite3_close(temp);
        return false;
    }
 
    bool users_exists = false;
    bool devices_exists = false;
    bool rooms_exists = false;
 
    rc = sqlite3_step(stmt);
    while (rc == SQLITE_ROW) {
        const char *tableName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        if (std::string(tableName) == "users") {
            users_exists = true;
        } else if (std::string(tableName) == "devices") {
            devices_exists = true;
        } else if (std::string(tableName) == "rooms") {
            rooms_exists = true;
        }
        rc = sqlite3_step(stmt);
    }
 
    sqlite3_finalize(stmt);
    sqlite3_close(temp);
 
    // Output results
    if (!users_exists) return false;
    if (!devices_exists) return false;
    if (!rooms_exists) return false;
 
    return true;
}

bool DBManager::check_db() {
    try
    {
        //check if db exists and is functional
        // else create if
        int rc = 0;
        std::ifstream f(DB_PATH);
        if(!f.good()){
            rc = this->create_database_config();
            if(rc == SQLITE_OK){
                rc = sqlite3_open(DB_PATH, &db);

                if (rc != SQLITE_OK) {
                    std::cerr << "Error opening SQLite database: " << sqlite3_errmsg(db) << std::endl;
                    logger->log_error("Error opening SQLite database: " + std::string(sqlite3_errmsg(db)));
                    sqlite3_close(db);
                    return false;
                }
                logger->log_warning("Database file missing. DB recreated from creation script.");
            }
        } else {
            if(!check_tables()){
                rc = std::remove(DB_PATH);
                if(rc!=0)
                    return false;
                rc = this->create_database_config();
                if(rc == SQLITE_OK){
                    rc = sqlite3_open(DB_PATH, &db);

                    if (rc != SQLITE_OK) {
                        std::cerr << "Error opening SQLite database: " << sqlite3_errmsg(db) << std::endl;
                        logger->log_error("Error opening SQLite database: " + std::string(sqlite3_errmsg(db)));
                        sqlite3_close(db);
                        return false;
                    }
                    logger->log_warning("Database file corrupted. DB recreated from creation script.");
                }
            }
        }

        return true;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        logger->log_error("Unexpected error checking DB integrity: " + std::string(e.what()));
        return false;
    }
}

DBManager *DBManager::GetDBInstance() {
    if(dbInstance == nullptr) {
        try
        {
            dbInstance = new DBManager();
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            dbInstance = nullptr;
        }
        return dbInstance;
    }
    
    return dbInstance;
}


void DBManager::destroyInstance() {

    if(dbInstance != nullptr) {
        delete dbInstance;
        dbInstance = nullptr;
        return;
    }
    return;
}

// Callback function for error messages
static int callback(void* NotUsed, int argc, char** argv, char** azColName) {
    return 0;
}

bool DBManager::insert_statement(std::string insert_statement, char* error_msg=NULL){
    if(dbInstance == nullptr) {
        logger->log_warning("DBManager::insert_statement - nullptr DB instance.");
        return false;
    }
    if(!check_db()){
        std::cerr << "DBManager::insert_statement - check_db failed: database file does not exists or cannot be created." << std::endl;
        return false;
    }
   
    // Execute INSERT statement
    char* errorMessage = nullptr;
    
    // Execute the INSERT statement
    int rc = sqlite3_exec(db, insert_statement.c_str(), callback, 0, &errorMessage);
    
    // Check for errors
    if (rc != SQLITE_OK) {
        std::cerr << "Error executing SQL statement: " << errorMessage << std::endl;
        if(error_msg!=NULL){
            error_msg=errorMessage;
        }
        else
            sqlite3_free(errorMessage);
        return false;
    }
    
    return true;
}

char*** DBManager::query(const std::string& sql_string, int* m_rows=0) {
    if(dbInstance == nullptr) {
        logger->log_warning("DBManager::query - nullptr instance.");
        *m_rows=-1;
        return NULL;
    }

    if(!check_db()){
        std::cerr << "DBManager::insert_statement - check_db failed: database file does not exists or cannot be created." << std::endl;
        return NULL;
    }
    
    
    char*** results = NULL;
    int rc;
    const char *sql = sql_string.c_str(); // Convert string to char
    sqlite3_stmt *stmt = NULL; // SQL statement

    /* Execute SQL statement */
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL); // Prepare statement

    if (rc != SQLITE_OK )
    { // Check error
        logger->log_error("DB error: sqlite3_prepare_v2 failed.");
        return NULL;
    }

    int cols = sqlite3_column_count(stmt); // Number of columns
    char *data = NULL; // data char pointer
    results = (char***)malloc(sizeof(char**));
    results[0] = (char**)malloc(cols*sizeof(char*));
    for (int i = 0; i < cols; i++)
    {
        // Print column name
        char* column_name = (char*)sqlite3_column_name(stmt,i);
        results[0][i] = strdup(column_name);
    }

    int rows = 1;
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {   
        char*** temp = (char***)realloc(results, ++rows*(sizeof(char**)));
        if(temp==NULL){
            free(results);
            *m_rows=0;
            return NULL;
        } else if(results!=temp){
            results=temp;
        }
        temp=NULL;

        // print each row
        results[rows - 1] = (char**)malloc(cols*sizeof(char*));
        for (int i = 0; i < cols; i++)
        {
            data = (char*)sqlite3_column_text(stmt, i);
            results[rows - 1][i]  = data ? strdup(data) : strdup("[NULL]");
        }
    }
    if(rows == 1){
        for(int i=0; i <cols;i++){
            free(results[0][i]);
        }
        free(results[0]);
        free(results);
        results = NULL;
    }

    /* Close Database */
    sqlite3_finalize(stmt);
    *m_rows = rows;
    return results;

}

int DBManager::create_database_config(){
    try
    {
        if (FILE *file = fopen(DB_PATH, "r")) {
            fclose(file);
            return SQLITE_OK;
        }
        
        int result = sqlite3_open(DB_PATH, &db);

        if (result == SQLITE_OK) {
            // Execute SQL script to create tables and schema
            
            std::fstream scriptFile;
            scriptFile.open(CREATION_SCRIPT_PATH);
            if (!scriptFile.is_open()) {
                std::cerr << "Error opening SQL script file: " << CREATION_SCRIPT_PATH << std::endl;
                logger->log_error("Error opening SQL script file: " + std::string(CREATION_SCRIPT_PATH));
                scriptFile.close();
                return SQLITE_ERROR;
            }

            std::string sqlQuery((std::istreambuf_iterator<char>(scriptFile)), std::istreambuf_iterator<char>());

            char* errorMessage = nullptr;
            int result = sqlite3_exec(db, sqlQuery.c_str(), nullptr, nullptr, &errorMessage);

            if (result != SQLITE_OK) {
                std::cerr << "Error executing SQL script: " << errorMessage << std::endl;
                logger->log_error("Error executing SQL script: " + std::string(errorMessage));
                sqlite3_free(errorMessage);
            }

            // Close the database
            sqlite3_close(db);

            if (result == SQLITE_OK) {
                return SQLITE_OK;
            } else {
                std::cerr << "Error creating database." << std::endl;
                logger->log_error("DBManager::create_database_config - Error creating database.");
                return SQLITE_ERROR;
            }
        } else {
            std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
            logger->log_error("DBManager::create_database_config - Error opening database: " + std::string(sqlite3_errmsg(db)));
            return SQLITE_ERROR;
        }
    }
    catch(const std::exception& e)
    {
        std::cerr <<"create_database_config error: "<< e.what() << '\n';
        logger->log_error("DBManager::create_database_config unexpected error - " + std::string(e.what()));
        return SQLITE_ERROR;
    }
}


std::vector<Room*> DBManager::MapRoomList() {
    if(dbInstance == nullptr) {
        std::cerr << "DBManager::MapRoomList - nullptr instance." << std::endl;
        logger->log_warning("DBManager::MapRoomList - nullptr instance.");
        return std::vector<Room*>();
    }
    if(!check_db()){
        std::cerr << "DBManager::MapRoomList - check_db failed: database file does not exists or cannot be created." << std::endl;
        logger->log_error("DBManager::MapRoomList - check_db failed: database file does not exists or cannot be created.");
        return std::vector<Room*>();
    }
    
    int rows;
    int columns=3;
    char*** room_details;
    std::vector<Room*> return_room_list;

    room_details = query("SELECT * from rooms", &rows);
    if(room_details!=NULL){
        for(int i = 1; i < rows; i++) {
            Room* room = new Room(room_details[i][0], room_details[i][1], room_details[i][2]);
            return_room_list.push_back(room);
        }
        for(int i=0; i < rows; i++){
            for(int j =0; j < columns; j++){
                free(room_details[i][j]);
            }
            free(room_details[i]);
        }
        free(room_details);
    }
    return return_room_list;
}

std::vector<Device*> DBManager::MapDeviceList() {
    if(dbInstance == nullptr) {
        std::cerr << "DBManager::MapDeviceList - nullptr instance." << std::endl;
        logger->log_warning("DBManager::MapDeviceList - nullptr instance.");
        return std::vector<Device*>();
    }
    if(!check_db()){
        std::cout << "DBManager::MapDeviceList - check_db failed: database file does not exists or cannot be created." << std::endl;
        logger->log_error("DBManager::MapDeviceList - check_db failed: database file does not exists or cannot be created.");
        return std::vector<Device*>();
    }
    
    int rows;
    int columns=5;
    char*** devices;
    std::vector<Device*> return_device_list = std::vector<Device*>();;
    devices = query("SELECT * from devices", &rows);
    if(devices!=NULL){
        for (int i = 1; i < rows; i++) {
            Device* new_device = nullptr;
	    std::string filename(CONFIG_DIR_PATH);
	    filename+=std::string(devices[i][1]);
	    filename+=".json";

	    std::ifstream f(filename.c_str());
	    if(!f.good())
		    continue;
            if(!strcmp(devices[i][4], "Smart_Bulb")) {
                new_device = new SmartBulb(devices[i][1], devices[i][0], devices[i][2]);
                new_device->set_device_type(devices[i][4]);
                new_device->load_json_config();
            } 
            if (!strcmp(devices[i][4], "Smart_Lock")) {
                new_device = new SmartLock(devices[i][1], devices[i][0], devices[i][2]);
                new_device->set_device_type(devices[i][4]);
                new_device->load_json_config();
            } 
            if (!strcmp(devices[i][4], "Air_Quality_Monitor")) {
                new_device = new AirQualityMonitor(devices[i][1], devices[i][0], devices[i][2]);
                new_device->set_device_type(devices[i][4]);
                new_device->load_json_config();        
            }
            return_device_list.push_back(new_device);
        }
        for(int i=0; i < rows; i++){
            for(int j =0; j < columns; j++){
                free(devices[i][j]);
            }
            free(devices[i]);
        }
        free(devices);
    }
    
    return return_device_list;
}

