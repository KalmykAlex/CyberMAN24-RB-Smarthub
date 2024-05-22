#pragma once
#include <iostream>
#include <string>
#include <Device.h>
#include <vector>

class Room
{
private:
    std::string description;
    std::string name;
    std::string room_id;  
public:

    Room();
    ~Room();
    Room(std::string room_id, std::string name, std::string description);
    std::vector<Device*> get_device_list();
    std::string get_room_description();
    std::string get_room_name();
    std::string get_room_id();    

};