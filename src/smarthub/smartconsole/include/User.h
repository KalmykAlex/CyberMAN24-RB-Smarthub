#pragma once
#include <iostream>
#include <vector>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "Room.h"

class User
{
private:
    std::string username;
    std::string password;
    
    int sock;
    bool LoggedIn = false;
    
    sockaddr* clientaddr;
    socklen_t addrlen;

    std::vector<Room*> rooms;
    std::vector<Device*> devices;
    char* ip=NULL;
public:
    User(){};
    User(int socket, sockaddr* client_addr, socklen_t addr_len);
    ~User();

    int get_socket();
    sockaddr* get_clientaddr();
    socklen_t get_addrlen();
    std::string get_username();
    std::string get_password();
    std::string get_ip_address();
    void set_ip_address(in_addr sin_addr);

    std::vector<Room*> get_rooms();
    std::vector<Device*> get_devices();
    void set_rooms(std::vector<Room*> all_rooms);
    void set_devices(std::vector<Device*> all_devices);
    void set_password(std::string passwd);
    void set_username(std::string uname);
    Room* find_device_location(std::string device_name);

    bool isLoggedIn();
    void login(std::string uname, std::string passwd);
    void logout();
    void update_data();

    std::string toString();
};