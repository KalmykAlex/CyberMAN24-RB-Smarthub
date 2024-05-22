#pragma once
#include "utils.h"
#include <string.h>

class Device
{

protected:

    /* Attributes */
    char m_name[DEFAULT_BUFFER_SIZE];
    char m_device_id[DEFAULT_BUFFER_SIZE];    
    bool m_isUp;
    char ip_address[16];
    char* status;
    char device_type[DEFAULT_BUFFER_SIZE];

public:

    /* Constructors */
    Device();
    Device(char* name, char* id, char* ip);
    virtual ~Device();


    void load_json_config();

    /* Getters */
    std::string get_device_name();
    std::string get_device_id();
    bool isUp(); 
    std::string get_ip_address();
    std::string get_status();
    std::string get_device_type();    

    /* Setters */
    void set_device_id(char* new_device_id);
    void set_device_name(char* name);
    void set_isUp(bool new_state);
    void set_ip_address(char* ip);
    void set_status(char* status);
    void set_device_type(const char* device_type);
    /* Methods for smartbulb */
    virtual void set_intensity(unsigned int new_intensity) = 0;
    virtual void set_voltage(unsigned int new_voltage) = 0;
    virtual void set_state(bool state) = 0;

    virtual unsigned int get_intensity() = 0;
    virtual bool isOn() = 0;
    virtual unsigned int get_voltage() = 0;

    /* Methods for AirQualityMonitor */
    virtual float get_humidity() = 0;
    virtual void set_humidity(float new_humidity_value) = 0;

    /* Methods for smart lock */
    virtual void set_lock(bool lock_state) = 0;
};
