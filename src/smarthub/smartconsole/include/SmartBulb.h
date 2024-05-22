#pragma once
#include "Device.h"
#include <iostream>
class SmartBulb: public Device
{

private:
    unsigned int m_intensity;
    unsigned int voltage;
    bool state;

public:

    SmartBulb();
    ~SmartBulb();
    SmartBulb(char* name, char* id, char* ip);
    
    unsigned int get_intensity() override;
    bool isOn() override;
    unsigned int get_voltage() override;

    void set_intensity(unsigned int new_intensity) override;
    void set_voltage(unsigned int new_voltage) override;
    void set_state(bool state) override;

    float get_humidity() override;
    void set_humidity(float new_humidity_value) override;

    void set_lock(bool lock_state) override;

};