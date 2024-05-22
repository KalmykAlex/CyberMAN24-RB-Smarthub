#include <AirQualityMonitor.h>

AirQualityMonitor::AirQualityMonitor() : Device() {
    this->humidity = 0;
}
AirQualityMonitor::~AirQualityMonitor(){
    this->humidity = 0;
}

AirQualityMonitor::AirQualityMonitor(char* name, char* id, char* ip)
    : Device(name, id, ip) {}

float AirQualityMonitor::get_humidity() {
    return this->humidity;
}

void AirQualityMonitor::set_humidity(float new_humidity_value) {
    this->humidity = new_humidity_value;
}


void AirQualityMonitor::set_intensity(unsigned int new_intensity) {
    throw std::logic_error("Not implemented method for this type of objectS");

}
void AirQualityMonitor::set_voltage(unsigned int new_voltage) {
    throw std::logic_error("Not implemented method for this type of objectS");

}
void AirQualityMonitor::set_state(bool state) {
    throw std::logic_error("Not implemented method for this type of objectS");

}
unsigned int AirQualityMonitor::get_intensity() {
    throw std::logic_error("Not implemented method for this type of objectS");

}
bool AirQualityMonitor::isOn() {
    throw std::logic_error("Not implemented method for this type of objectS");

}
unsigned int AirQualityMonitor::get_voltage() {
    throw std::logic_error("Not implemented method for this type of objectS");    
}

void AirQualityMonitor::set_lock(bool lock_state) {
    throw std::logic_error("Not implemented method for this type of objectS");    

}