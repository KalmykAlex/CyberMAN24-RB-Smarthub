#include "SmartLock.h"

SmartLock::SmartLock() : Device() {}
SmartLock::~SmartLock() {}

SmartLock::SmartLock(char* name, char* id, char* ip) : Device(name, id, ip) {
    this->m_isLocked = false;
}

bool SmartLock::isLocked() {
    return m_isLocked;
}

void SmartLock::set_lock(bool new_status) {
    this->m_isLocked = new_status;
}


void SmartLock::set_intensity(unsigned int new_intensity){
    throw std::logic_error("Not implemented method for this type of object");
}
void SmartLock::set_voltage(unsigned int new_voltage){
    throw std::logic_error("Not implemented method for this type of object");
}
void SmartLock::set_state(bool state){
    throw std::logic_error("Not implemented method for this type of object");
}

unsigned int SmartLock::get_intensity(){
    throw std::logic_error("Not implemented method for this type of object");
}
bool SmartLock::isOn(){
    throw std::logic_error("Not implemented method for this type of object");
}
unsigned int SmartLock::get_voltage(){
    throw std::logic_error("Not implemented method for this type of object");
}


float SmartLock::get_humidity(){
    throw std::logic_error("Not implemented method for this type of object");
}
void SmartLock::set_humidity(float new_humidity_value){
    throw std::logic_error("Not implemented method for this type of object");
}
