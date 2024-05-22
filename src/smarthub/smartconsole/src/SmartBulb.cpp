#include "SmartBulb.h"

SmartBulb::SmartBulb() : Device()
{
    this->m_intensity = 0;
    this->voltage = 0;
}

SmartBulb::~SmartBulb()
{
    this->m_intensity = 0;
    this->voltage = 0;
}


SmartBulb::SmartBulb(char *name, char *id, char* ip) : Device(name, id, ip)
{
    this->m_intensity = 0;
    this->voltage = 0;
}

unsigned int SmartBulb::get_intensity()
{
    return this->m_intensity;
}

unsigned int SmartBulb::get_voltage()
{
    return this->voltage;
}

void SmartBulb::set_intensity(unsigned int new_intensity)
{
    this->m_intensity = new_intensity;
}
void SmartBulb::set_voltage(unsigned int new_voltage)
{
    this->voltage = new_voltage;
}

void SmartBulb::set_state(bool state)
{
    this->state = state;
}

bool SmartBulb::isOn()
{
    return this->state;
}

float SmartBulb::get_humidity()
{
    throw std::logic_error("Not implemented method for this type of objectS");
}

void SmartBulb::set_humidity(float new_humidity_value)
{
    throw std::logic_error("Not implemented method for this type of object");
}

void SmartBulb::set_lock(bool lock_state) {
    throw std::logic_error("Not implemented method for this type of object");
    
}