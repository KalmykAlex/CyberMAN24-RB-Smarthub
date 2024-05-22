#include "Room.h"
#include "DBManager.h"
#include "SmartBulb.h"
#include "SmartLock.h"
#include "AirQualityMonitor.h"

Room::Room() {

}

Room::~Room() {
}

Room::Room(std::string room_id, std::string name, std::string description) {
    this->name = name;
    this->description = description;
    this->room_id = room_id;
}

std::vector<Device*> Room::get_device_list() {
    std::vector<Device*> device_list;
    DBManager* db = DBManager::GetDBInstance();
    std::string device_query = "SELECT * from devices WHERE devices.room_id = " + this->room_id;

    int device_rows=0;
    char*** room_devices = db->query(device_query, &device_rows);
        
    for (int i = 1; i < device_rows; i++) {
        Device* new_device = nullptr;
        if(!strcmp(room_devices[i][4], "Smart_Bulb")) {
            new_device = new SmartBulb(room_devices[i][1], room_devices[i][0], room_devices[i][2]);
        } 
        if (!strcmp(room_devices[i][4], "Smart_Lock")) {
            new_device = new SmartLock(room_devices[i][1], room_devices[i][0], room_devices[i][2]);
        } 
        if (!strcmp(room_devices[i][4], "Air_Quality_Monitor")) {
            new_device = new AirQualityMonitor(room_devices[i][1], room_devices[i][0], room_devices[i][2]);
        }
        device_list.push_back(new_device);
    }

    int columns = 5; // NUmber of columns from devices table
    if(room_devices!=NULL){
        for(int i=0; i < device_rows;i++){
            for(int j=0;j<columns;j++){
                free(room_devices[i][j]);
            }
            free(room_devices[i]);
        }
        free(room_devices);
    }


    return device_list;
}


std::string Room::get_room_description() {
    return this->description;
}

std::string Room::get_room_name() {
    return this->name;
}

std::string Room::get_room_id() {
    return this->room_id;
}