#include "utils.h"
#include "Device.h"
#include <stdexcept>

pthread_mutex_t log_mutex = PTHREAD_MUTEX_INITIALIZER;

Json::Value parse_json(const char* json_file_name) 
{
    std::ifstream device_type_file(json_file_name);

    if (!device_type_file.is_open()) {
        std::cerr << "Failed to open json device file!" << std::endl;
        throw std::runtime_error("Failed to open json device file!");
    }

     // Read the entire file content into a string
    std::string json_content((std::istreambuf_iterator<char>(device_type_file)),
                            std::istreambuf_iterator<char>());

     // Close the file
    device_type_file.close();
    
     // Parse the JSON string
    Json::Value root;
    Json::CharReaderBuilder builder;
    Json::CharReader *reader = builder.newCharReader();
    std::string errors;


    bool parsingSuccessful = reader->parse(json_content.c_str(),
                                                json_content.c_str() + json_content.size(),
                                                &root, &errors);
    delete reader;

    if (!parsingSuccessful) {
        std::cerr << "Failed to parse JSON device details file: " << errors << std::endl;
        throw std::runtime_error("Failed to parse JSON device details file: " + errors);
    }
        
    return root;
}

void write_json_to_file(const Json::Value& json_value, std::string device_name) {
    // Convert Json::Value to a string
    Json::StreamWriterBuilder builder;
    std::string json_str = Json::writeString(builder, json_value);
    std::string file_path = CONFIG_DIR_PATH + device_name + ".json";
    // Write the string to a file
    std::ofstream output_file(file_path);
    if (output_file.is_open()) {
        output_file << json_str;
        output_file.close();
    } else {
        std::cerr << "Unable to open file: " << file_path << std::endl;
    }
}

std::string show_general_edit_options() {

    std::string result_string = "";
    result_string += "Press 1 to modify ip address\n";
    result_string += "[+] Press 2 to modify status\n";
    return result_string;

}

std::string show_edit_option_smart_bulb() {
    
    std::string result_string = show_general_edit_options();
    result_string += "[+] Press 3 to change intensity\n";
    result_string += "[+] Press 4 to change voltage\n";
    result_string += "[+] Input: ";
    return result_string;
}

std::string show_edit_option_smart_lock() {

    std::string result_string = show_general_edit_options();
    result_string += "[+] Press 3 to change lock state\n";
    result_string += "[+] Input: ";
    return result_string;
}
