#pragma once
#include "Device.h"
#include <iostream>

class AirQualityMonitor : public Device
{

private:
    float humidity;

public:

    AirQualityMonitor();
    ~AirQualityMonitor();
    AirQualityMonitor(char* name, char* id, char* ip);
    float get_humidity();
    void set_humidity(float new_humidity_value);

    /* Not implemented methods */
    void set_intensity(unsigned int new_intensity);
    void set_voltage(unsigned int new_voltage);
    void set_state(bool state);
    unsigned int get_intensity();
    bool isOn();
    unsigned int get_voltage();
    void set_lock(bool new_state);


};