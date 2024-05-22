#include "Device.h"

Device::Device() {
    memset(m_name, '\0', DEFAULT_BUFFER_SIZE);
    memset(m_device_id, '\0', DEFAULT_BUFFER_SIZE);
    memset(ip_address, '\0', 16);
    
    memcpy(m_name, "None", 4);
    memcpy(m_device_id, "None", 4);
    memcpy(ip_address, "None", 4);
}

Device::Device(char* name, char* id, char* ip) {
    memset(m_name, '\0', DEFAULT_BUFFER_SIZE);
    memset(m_device_id, '\0', DEFAULT_BUFFER_SIZE);
    memset(ip_address, '\0', 16);

    memcpy(m_name, name, (strlen(name) > DEFAULT_BUFFER_SIZE) ? DEFAULT_BUFFER_SIZE : strlen(name));
    memcpy(m_device_id, id, (strlen(id) > DEFAULT_BUFFER_SIZE) ? DEFAULT_BUFFER_SIZE : strlen(id));
    memcpy(ip_address, ip, 16);
    this->status = strdup("ONLINE");
}

Device::~Device() {
    memset(this->m_name, '\0', DEFAULT_BUFFER_SIZE);
    memset(this->m_device_id, '\0', DEFAULT_BUFFER_SIZE);
    memset(this->ip_address, '\0', IP4_SIZE);
    free(this->status);
}

void Device::load_json_config() {
    try
    {
        std::string path = CONFIG_DIR_PATH + std::string(this->get_device_name()) + ".json";
        Json::Value device_json = parse_json(path.c_str());

        if(!strcmp(device_json["status"].asCString(), "ONLINE")) {
            this->set_isUp(true);
        } else {
            this->set_isUp(false);
        }

        if(!strcmp(this->device_type, "Smart_Bulb")) {
            this->set_intensity(atoi(device_json["intensity"].asCString()));
            this->set_voltage(atoi(device_json["voltage"].asCString())); 
        } else if (!strcmp(this->m_name, "Smart_Lock")) {
            if(!strcmp(device_json["locked_state"].asCString(), "LOCKED"))
                this->set_lock(true);
            else 
                this->set_lock(false);
        } else if(!strcmp(this->m_name, "Air_Quality_Monitor")) {
            this->set_humidity(atof(device_json["humidity"].asCString()));
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << "load_json_config error: " << e.what() << '\n';
        this->set_isUp(false);
        

        if(!strcmp(this->device_type,"Smart_Bulb")) {
            this->set_intensity(0);
            this->set_voltage(0); 
        } else if (!strcmp(this->m_name, "Smart_Lock")) {
            this->set_lock(false);
        } else if(!strcmp(this->m_name, "Air_Quality_Monitor")) {
            this->set_humidity(0);
        }
    }
}

std::string Device::get_device_id() {
    return m_device_id;
}

std::string Device::get_device_name() {
    return this->m_name;
}

std::string Device::get_ip_address() {
    return std::string(this->ip_address);
}

std::string Device::get_status() {
    return this->status;
}

void Device::set_status(char* status) {
    this->status=strdup(status);
}

bool Device::isUp() {
    return this-> m_isUp;
}

void Device::set_device_id(char* m_device_id) {
    memset(m_device_id, '\0', DEFAULT_BUFFER_SIZE);
    memcpy(m_device_id, m_device_id, (strlen(m_device_id) > DEFAULT_BUFFER_SIZE) ? DEFAULT_BUFFER_SIZE : strlen(m_device_id));
}

void Device::set_device_name(char* new_device_name) {
    memset(m_name, '\0', DEFAULT_BUFFER_SIZE);
    memcpy(m_name, new_device_name, (strlen(new_device_name) > DEFAULT_BUFFER_SIZE) ? DEFAULT_BUFFER_SIZE : strlen(new_device_name));
}

void Device::set_ip_address(char* ip) {
    memset(ip_address, '\0', 16);
    memcpy(ip_address, ip, (strlen(ip) > 16) ? 16 : strlen(ip));

}

void Device::set_isUp(bool new_state) {
    this->m_isUp = new_state;
}

std::string Device::get_device_type() {
    return this->device_type;
}

void Device::set_device_type(const char* device_type) {
    memset(this->device_type, '\0', DEFAULT_BUFFER_SIZE);
    memcpy(this->device_type, device_type, (strlen(device_type) > DEFAULT_BUFFER_SIZE) ? DEFAULT_BUFFER_SIZE : strlen(device_type));
}
