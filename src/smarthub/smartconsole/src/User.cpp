#include "User.h"
#include "DBManager.h"

User::~User(){
    this->sock=-1;
    this->logout();
    free(this->ip);
}

void User::login(std::string uname, std::string passwd){
    this->LoggedIn = true;
    this->username = uname.c_str();
    this->password = passwd.c_str();
}

void User::logout(){
    this->LoggedIn = false;
    this->username = "";
    this->password = "";
    if(!this->rooms.empty()){
        for (Room* room : this->rooms) {
            delete room;
        }
        rooms.clear();
    }
    if(!this->devices.empty()){
        for (Device* device : devices) {
            delete device;
        }
        devices.clear();
    }
}

bool User::isLoggedIn(){
    return this->LoggedIn;
}

User::User(int socket, sockaddr* client_addr, socklen_t addr_len)
: sock(socket), clientaddr(client_addr), addrlen(addr_len){
}

int User::get_socket(){
    return this->sock;
}

sockaddr* User::get_clientaddr(){
    return this->clientaddr;
}

socklen_t User::get_addrlen(){
    return this->addrlen;
}

std::string User::get_username(){
    return this->username;
}

std::string User::get_password(){
    return this->password;
}

std::vector<Room*> User::get_rooms(){
    if(this->isLoggedIn() == true){
        return this->rooms;
    }
    return std::vector<Room*>();
}

std::vector<Device*> User::get_devices(){
    if(this->isLoggedIn() == true){
        return this->devices;
    }
    return std::vector<Device*>();
}
void User::set_rooms(std::vector<Room*> all_rooms){
    if(this->isLoggedIn() == true){
        this->rooms = all_rooms;
    }
}
void User::set_devices(std::vector<Device*> all_devices){
    if(this->isLoggedIn() == true){
        this->devices = all_devices;
    }
}

Room* User::find_device_location(std::string device_name){
    bool break_for=false;
    Room* found_room=nullptr;
    std::vector<Device*> devices;

    for(Room* r: this->rooms){   
        std::vector<Device*> devices = r->get_device_list(); 
        for(Device* d: devices){
            if(d->get_device_name() == device_name){
                found_room = r;
                break_for=true;
                break;
            }
        }
        for(Device* d: devices){
            delete d;
        }
        devices.clear();

        if(break_for)
            break;
    }
    return found_room;
}

void User::update_data(){
    DBManager* db = DBManager::GetDBInstance();
    this->devices=db->MapDeviceList();
    this->rooms=db->MapRoomList();
    db->destroyInstance();
}

void User::set_password(std::string passwd){
    this->password = passwd;
}
void User::set_username(std::string uname){
    this->username = uname;
}

std::string User::get_ip_address(){
    return this->ip;
}

void User::set_ip_address(in_addr sin_addr){
    char str[INET_ADDRSTRLEN];
    if(getpeername(this->sock, (struct sockaddr*)this->clientaddr, &this->addrlen)==0){
        inet_ntop(AF_INET, &sin_addr, str, INET_ADDRSTRLEN);
    }
    this->ip = strdup(str);
}

std::string User::toString(){
    std::string profile="";
    profile+="[+] Username: "+this->username+"\n";
    profile+="[+] Password: ";
    for(char c:this->password){
        profile+="*";
    }
    profile+='\n';
    return profile;
}
