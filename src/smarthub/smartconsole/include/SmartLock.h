#pragma once
#include <iostream>
#include "Device.h"

class SmartLock : public Device {

private:
    bool m_isLocked;
public:

    /* Constructors */
    SmartLock();
    ~SmartLock();
    SmartLock(char* name, char* id, char* ip);

    
    bool isLocked();
    void set_lock(bool status);

    /* Methods for smartbulb */
    void set_intensity(unsigned int new_intensity);
    void set_voltage(unsigned int new_voltage);
    void set_state(bool state);

    unsigned int get_intensity();
    bool isOn();
    unsigned int get_voltage();

    /* Methods for AirQualityMonitor */
    float get_humidity();
    void set_humidity(float new_humidity_value);

    
};