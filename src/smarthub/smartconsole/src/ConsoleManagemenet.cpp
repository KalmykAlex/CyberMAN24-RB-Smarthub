#include <unistd.h>
#include <stdio.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <unordered_map>
#include <openssl/md5.h>

#include "json.h"
#include "ConsoleManagement.h"
#include "UserDisconnected.h"


int ConsoleManagement::update_smart_bulb(Device* device, int option, int connfd) {

    int rows=0;
    std::string message = "Enter new value: ";
    std::string response = take_input(connfd, message);
    std::string json_path = CONFIG_DIR_PATH + std::string(device->get_device_name()) + ".json";
    Json::Value json_device = parse_json(json_path.c_str());
    
    std::string query = "";
    std::string query_1 = "UPDATE devices SET ";
    std::string query_2 = "";
    std::string query_3 = "WHERE device_name =";
    query_3 +=  " \""; 
    query_3 +=  std::string(device->get_device_name());
    query_3 += + "\";";
    
    switch (option)
    {
    case 1:
    {
        device->set_ip_address((char*)response.c_str());
        query_2 = "ip_address = ";
        query_2 += ('\"' + response + "\" ");            
        query = query_1 + query_2 + query_3;
        json_device["ip_address"] = response;

        db->query(query, &rows);
        break;
    }
    case 2:
    {   
        if(response != "ONLINE" && response != "OFFLINE")
            return -1;

        device->set_status((char*) response.c_str());
        json_device["status"] = response;
        
        break;
    }
    case 3:
    {
        int intensity = atoi(response.c_str());
        if (intensity < 0 || intensity > 100) 
            return -1;
        
        device->set_intensity(intensity);
        json_device["intensity"] = response;
        break;
    }
    case 4:
    {
        int voltage = atoi(response.c_str());
        device->set_voltage(voltage);
        json_device["voltage"] = response;
        break;
    }
    default:
        break;
    }

    write_json_to_file(json_device, std::string(device->get_device_name()));
    
    return 0;
}

std::string ConsoleManagement::format_string(const std::string& input){
    if(input.empty())
        return "";
    std::string result = input;
    std::transform(result.begin(), result.end(), result.begin(), [](char c){
        return (c=='_') ? ' ' : c;
    });
    result[0] = std::toupper(result[0]);

    return result;
}

std::string ConsoleManagement::get_device_specific_data(std::string device_name){

    try
    {
        Json::Value root = parse_json(GLOBAL_CONFIG_JSON_PATH);

        std::string file_path = std::string(CONFIG_DIR_PATH)+device_name+".json";
        Json::Value device_root = parse_json(file_path.c_str());

        if (!root or !device_root) {
            return "";
        } else {
            std::string response="";
            std::string type = device_root["device_type"].asString();
            const Json::Value device_data = root["device_types"];
            

            for (int index = 0; index < device_data.size(); ++index){
                if(device_data[index]["type"].asString() == type){
                    const Json::Value fields = device_data[index]["required_data"];
                    for(int j=0; j < fields.size(); j++){
                        std::string value = "";
                        std::string field = fields[j].asString();
                        if (field != "device_name" and field != "ip_address" and !field.empty() and !device_root[field.c_str()].asString().empty()){
                            int len = field.length();
                            std::string spaces="";
                            for(int i=0; i < 20-(len+1); i++){
                                spaces+=" ";
                            }

                            value += "[+]  " + format_string(field) + ":" + spaces;
                            value += device_root[field.c_str()].asString();
                            value += "\n";
                        }
                        response+=value; 
                    } 
                    break; 
                }
            }
            return response;
        }       
        return "";
    }
    catch(const std::exception& e)
    {
        logger->log_error("get_device_specific_data: " + std::string(e.what()));
        return "";
    }
}

int ConsoleManagement::update_smart_lock(Device* device, int option, int connfd) {

    int rows=0;
    std::string message = "Enter new value: ";
    std::string response = take_input(connfd, message);
    std::string json_path = CONFIG_DIR_PATH + std::string(device->get_device_name()) + ".json";
    Json::Value json_device = parse_json(json_path.c_str());
    
    std::string query = "";
    std::string query_1 = "UPDATE devices SET ";
    std::string query_2 = "";
    std::string query_3 = "WHERE device_name =";
    query_3 +=  " \""; 
    query_3 +=  std::string(device->get_device_name());
    query_3 += + "\";";
    
    switch (option)
    {
    case 1:
    {
        device->set_ip_address((char*)response.c_str());
        query_2 = "ip_address = ";
        query_2 += ('\"' + response + "\" ");            
        query = query_1 + query_2 + query_3;
        json_device["ip_address"] = response;
        
        db->query(query, &rows);
        break;
    }
    case 2:
    {
        device->set_status((char*) response.c_str());
        json_device["status"] = response;
        break;
    }
    case 3:
    {
        json_device["locked_state"] = response;
        break;
    }

    default:
        return -1;
    }
    write_json_to_file(json_device, std::string(device->get_device_name()));

    return 0;

}

int ConsoleManagement::update_air_quality(Device* device, int option, int connfd) {

    int rows=0;
    std::string message = "Enter new value: ";
    std::string response = take_input(connfd, message);
    std::string json_path = CONFIG_DIR_PATH + std::string(device->get_device_name()) + ".json";
    Json::Value json_device = parse_json(json_path.c_str());

    std::string query = "";
    std::string query_1 = "UPDATE devices SET ";
    std::string query_2 = "";
    std::string query_3 = "WHERE device_name =";
    query_3 +=  " \""; 
    query_3 +=  std::string(device->get_device_name());
    query_3 += + "\";";

    switch (option)
    {
        case 1:
        {
            device->set_ip_address((char*)response.c_str());
            query_2 = "ip_address = ";
            query_2 += ('\"' + response + "\" ");            
            query = query_1 + query_2 + query_3;
            json_device["ip_address"] = response;

            db->query(query, &rows);
            break;
        }
        case 2:
        {
            device->set_status((char*) response.c_str());
            json_device["status"] = response;

            break;
        }
        default:
        {
            return -1;
        }
    }
    return 0;
}

bool device_exists(char* device_name, char** error_message){
    std::string file_path = std::string(CONFIG_DIR_PATH) + device_name+".json";
    std::string command = "stat "+ file_path;
    std::string output;

    // Open a pipe to the command and read its output
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) {
        std::cerr << "Error executing command." << std::endl;
        fclose(pipe);
	return true;
    }

    char buffer[256];
    while (!feof(pipe)) {
        if (fgets(buffer, 128, pipe) != nullptr)
            output += buffer;
    }
    pclose(pipe);
    *error_message = strdup(output.c_str());

    int result = system(command.c_str());


    if (result == 0) {
        int check=0;
        std::string query = "SELECT * FROM devices WHERE device_name=\'"+std::string(device_name)+'\'';
        char*** results = db->query(query.c_str(), &check);
        int columns = 5; // NUmber of columns from devices table
        if(results!=NULL){
            for(int i=0; i < check;i++){
                for(int j=0;j<columns;j++){
                    free(results[i][j]);
                }
                free(results[i]);
            }
            free(results);
        }


        if(check==1){
            std::remove(file_path.c_str());
            return false;
        }

        return true;
    } else {
        return false;
    }

    return false;
}

std::string md5_to_hex(const unsigned char* md5_hash) {
    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    for (int i = 0; i < MD5_DIGEST_LENGTH; ++i) {
        ss << std::setw(2) << static_cast<unsigned>(md5_hash[i]);
    }
    return ss.str();
}

ConsoleManagement::ConsoleManagement() {
    db = DBManager::GetDBInstance();
    logger = Logger::GetInstance();
}

Response ConsoleManagement::EvaluateLogin(std::string username, std::string password){
    try
    {
        if(username.compare("anonymous") == 0){
            logger->log_info("Anonymous login.");
            return Response("Anonymous login!\n\033[36m[+] Hello stranger! :)\n[+] .help - show available options.\n", 200);
        }

        db = DBManager::GetDBInstance();
        int rows;

        char*** result = db->query("SELECT username,password FROM users WHERE username = \"" + username + "\" and password = \""+ password +"\";", &rows);
        if (result == NULL){
            return Response("\033[31mLogin failed!\n\033[36m", 666);
        }

        std::string welcome_msg = "Welcome, " + std::string(result[1][0]) + "!\n[+]\n[+] Use .help command to see the available options.\n";

        for(int i=0; i < rows; i++){
            for(int j=0; j < 2; j++){
                free(result[i][j]);
            }
            free(result[i]);
        }
        free(result);
        return Response(welcome_msg.c_str(), 200);
    }
    catch(const std::exception& e)
    {
        std::cerr << "EvaluateLogin error: "<<e.what() << '\n';
        std::string response="\033[31mLogin failed:"+std::string(e.what())+"\n\033[36m";
        return Response(response.c_str(), 666);
    }
}

Response ConsoleManagement::EvaluateAnonCommand(char* command, User& user){
    if (strcmp(command, ".help") == 0){
        strcpy(command, ".help anon");
        return EvaluateCommand(command, user); 
    }

    if(strstr(command, ".show")){
        strcpy(command + strlen(command), " anon");
        return EvaluateCommand(command, user); 
    }

    if (std::string(command) == ".logout")
       return EvaluateCommand(command, user); 

    return Response("Unknown command, use help to get the default command list.\n", 200);
}

Response ConsoleManagement::EvaluateCommand(const char *command, User& user)
{
    int rc=0;
    db = DBManager::GetDBInstance();
    int connfd = user.get_socket();
    if(!strcmp(command, ".profile")) {

        std::string user_profile = "\n[+] Profile information:\n"+user.toString();
        return Response(user_profile.c_str(), 200);
    }

    if(strcmp(command,".edit")==0){
        return Response("Edit device help menu\n[+] Usage: .edit <device>\n", 200);  
    }
    if(strstr(command, ".edit")) {

        std::string prompt;
        std::string response;
        char *cmd = strtok((char*)command, " ");
        char* arg = strtok(NULL, " ");

        if(!strcmp(arg, "device")){

            std::vector<Device*> device_list =  user.get_devices();
            prompt = "Available devices for edit are: \n";

            for(int i = 0; i < device_list.size(); i++) {
                prompt += "[+]  - " + std::string(device_list[i]->get_device_name());
                prompt += "\n";
            }
            prompt += "[+] Enter the name of device you want to edit: ";

            response = take_input(connfd, prompt);
	    if(response.find("Smart_Bulb_0021") != std::string::npos){
		    return Response("Don't try funny things!;)\n", 200);
	    }
	    bool found = false;
            for (long unsigned int i = 0; i < device_list.size(); i++) {
                if(!response.compare(device_list[i]->get_device_name())) {
                    if(!device_list[i]->get_device_type().compare("Smart_Bulb")) {
                            response = take_input(connfd, show_edit_option_smart_bulb());
                            rc = update_smart_bulb(device_list[i], atoi(response.c_str()), connfd);
                            found = true;
			    break;
                    } else if (!device_list[i]->get_device_type().compare("Smart_Lock")) {   
                           response = take_input(connfd, show_edit_option_smart_lock());
                           rc = update_smart_lock(device_list[i], atoi(response.c_str()), connfd);
			   found = true;
                           break;

                    } else if(!device_list[i]->get_device_type().compare("Air_Quality_Monitor")) {
                            response = take_input(connfd, show_general_edit_options()+"[+] Input: ");
                            rc = update_air_quality(device_list[i], atoi(response.c_str()), connfd);
			    found = true;
                            break;
                    }
                }
            }

		if (found == false)
			return Response("[-] Device not found!\n", 666);

        } else return Response("[-] Unknown argument for command edit\n", 666);
        if(prompt.empty())
            prompt="An error has occured. Try again later! :(\n";
        if(!rc)
            return Response("Updated succesfully\n", 200);

    }
  
    if(strcmp(command,".add")==0){
        const char* response = "Adds a new user or device in the system:\n[+] - .add device - goes into the device registration menu.\n[+] - .add user - goes into user registration menu\n";
        return Response(response, 200);
    } 
    if (strstr(command, ".add")){
        char *cmd = strtok((char*)command, " ");
        char* arg = strtok(NULL, " ");
        
        if(!strcmp(arg, "device")){
            
            std::string prompt;
            std::string errors;
            
            Json::Value root = parse_json(GLOBAL_CONFIG_JSON_PATH);

            if (!root) {
                return Response("Failed to parse GLOBAL_CONFIG_JSON_PATH", 200);
            } else {
                
                // root - contains all the data from device_types.json
                // from device_types field read all the existent types and send them to the user
                
                std::string types_string;
                const Json::Value device_types = root["device_types"];
                
                for ( int index = 0; index < device_types.size(); ++index ){
                    types_string+= "[+] - " + device_types[index]["type"].asString() + "\n";
                }

                prompt+= "Available device types:\n";
                prompt+= types_string;
                prompt+= "[+] Device type: ";

                // Reads the device type from the user
                std::string device_type = take_input(connfd, prompt);
                // device_types.json also contains all the required fields for every device
                // for the device selected by the user collect all the required information

                for (unsigned int index = 0; index < device_types.size(); ++index ){
                    // If the user input type is found in the device_type file
                    // Collect data for every required fields and save it in a hash map.
                    if(device_type == device_types[index]["type"].asString()) {

                        const Json::Value fields = device_types[index]["required_data"];
                        std::unordered_map<std::string, std::string> data;
                        

                        for (unsigned int j = 0; j < fields.size(); ++j ) {
                            
                            prompt= "  " + fields[j].asString() +": ";
                            std::string field = take_input(connfd, prompt);
                            data[fields[j].asString()] = field;
                        }

                        std::string room_name = take_input(connfd, "room_name: ");

                        char* message = NULL;
                        if(device_exists((char*)data["device_name"].c_str(), &message)){
                            std::string output = "Device already exists " + (message==NULL ? "\n" : "\n[+] Error: \n" + std::string(message));
                            return Response(output.c_str(), 200);
                        }
                        free(message);

                        std::string insert_query = "INSERT INTO devices(device_name, ip_address, room_id, device_type) VALUES(";
                        insert_query += "\""+data["device_name"]+"\",";
                        insert_query += "\""+data["ip_address"]+"\",";
                        insert_query += "(SELECT roomid FROM rooms WHERE rooms.name == \""+ room_name+ "\"),";
                        insert_query += "\""+device_type+"\"";
                        insert_query += ")";
                        
                        char* error = NULL;
                        bool rc = db->insert_statement(insert_query, error);
                        if(rc == false){
                            return Response("An error has occurred. Try again later!\n", 400);
                        }

                        // Create json file for device
                        // Build json content and save it to file
                        std::string device_json="{\n  ";
                        device_json+="\t\"device_type\": \""+device_type+"\",\n";
                        for (auto it = data.begin(); it != data.end(); ++it) {
                            const auto& pair = *it;
                            device_json+= "\t\"" + pair.first + "\": \"" + pair.second + "\"," + "\n";
                        }
                        device_json+="\t\"status\":\"ONLINE\"\n";
                        device_json+="\n}";
                        
                        std::string filename = CONFIG_DIR_PATH + data["device_name"] + ".json";
                        std::ofstream outputFile(filename);

                        if (!outputFile.is_open()) {
                            std::cerr << "Failed to open file: " << filename << std::endl;
                        }
                        outputFile << device_json;
                        outputFile.close();

                        user.update_data();               
                        return Response("Device succesfully register!\n",200);
                    }
                }
            }

            return Response("An error has occurred. Try again later!\n", 400);
        }

        if(!strcmp(arg, "user")){
            try
            {
                std::string prompt="";
                std::string new_username = "";
                std::string password= "";
                std::string password_conf = "";

                int result = 0;
                
                do
                {
                    if(result>1)
                        prompt="User already exists.\n[+] Input new username: ";
                    else 
                        prompt+="Input new username: ";
                    new_username = take_input(connfd, prompt);
                    std::string query="SELECT * FROM users WHERE username = \""+new_username+"\"";
                    
                    int columns=3;
                    char*** results = db->query(query, &result);
                    if(results!=NULL){
                        for(int i=0;i<result;i++){
                            for(int j=0; j < columns; j++){
                                free(results[i][j]);
                            }
                            free(results[i]);
                        }
                        free(results);
                    }
                    
                } while (result > 1);
                
                int retry = 0;
                do{
                    if(retry > 0){
                        prompt="Passwords doesn't match!\n[+] Password:";
                    } else {
                        prompt="Password: ";
                    }
                    
                    password= take_input(connfd, prompt);
                    prompt="Password confirmation: ";
                    password_conf = take_input(connfd, prompt);
                    ++retry;
                }while(password!=password_conf);


                std::string insert_query="INSERT INTO users(username, password) VALUES(";
                insert_query+="\""+new_username+"\",";
                insert_query+="\""+password+"\"";
                insert_query+=")";
                char* error = NULL;
                bool rc = db->insert_statement(insert_query, error);
                if(rc == false){
                    throw std::runtime_error("Insert error! Try again later.");
                }

                return Response("User succesfully register!\n",200);
            }
            catch(const std::exception& ex)
            { 
                std::cout << "ConsoleManagement: " << ex.what() << std::endl;
                throw ex;
            }
            catch(...)
            {
                std::cout << "ConsoleManagement: Dude, how do you even got here..."<<std::endl;
            }
        }

        return Response("Unknown command, use help to get the default command list.\n", 200);
    }
    
    if(!strcmp(command, ".help")) {
        const char* response = "Available commands:\n\033[36m[+] \u001b[33m.help - shows available commands\n\033[36m[+] \u001b[33m.show <devices|users|rooms>\n\033[36m[+] \u001b[33m.add <user|device>\n\033[36m[+] \u001b[33m.edit <device>\n\033[36m[+] \u001b[33m.logout\n\033[36m";

        return Response(response, 200);
    }
    if(!strcmp(command, ".help anon")) {
        const char* response = "Available commands:\n\033[36m[+] \u001b[33m.help - shows available commands\n\033[36m[+] \u001b[33m.show - just \"show\" for you ;)\n\033[36m[+] \u001b[33m.logout\n\033[36m";

        return Response(response, 200);
    }

    if(!strcmp(command, ".logout")) {
        const char* response = "User logged out!\n[+] \n";
        user.logout();

        return Response(response, 200);
    }

    if(strcmp(command,".show")==0){
        return Response("Usage: .show <devices|users|rooms>\n", 200);
    }
    if (std::string(command).substr(0, 5)==".show") {
        std::string response = "";

        std::string arg = std::string(command).substr(strlen(".show")+1, strlen(command));
        if(arg.find("users")!=std::string::npos){
            
            response += "Users:\n";
            std::string query = "SELECT * FROM "+ arg;
            int user_rows=0;
            char*** users = db->query(query, &user_rows);
            
            for(int i=1;i<user_rows;i++) {
                response += "[+] -"+std::string(users[i][1])+":"+std::string(users[i][2])+'\n';
            }  
            
            return Response(response.c_str(), 200);
        }
        if(arg.find("devices")!=std::string::npos){
            response += "Devices:\n";
    
            for(Device* d: user.get_devices()){
                if(d == nullptr){
                    continue;
                }
                response += "[+] \n";
                response += "[+]  Device Name:        " + d->get_device_name() + '\n';
                response +=  "[+]  IP Address:         " + d->get_ip_address() + '\n';
                Room* location = user.find_device_location(d->get_device_name());
                response +=    "[+]  Location:           " + ((location == nullptr) ? "UNKNOWN" : location->get_room_name()) +'\n';
                response += get_device_specific_data(d->get_device_name());
                response +=      "[+]  Status:             \u001b[32m" + d->get_status() + "\033[36m\n";
                }
            
            return Response(response.c_str(), 200);
        }

        if(arg.find("rooms")!=std::string::npos){
            response += "Rooms:\n";
            for(auto r: user.get_rooms()){
                response += "[+]  - " + r->get_room_name() + '\n';
            }
            response+="[+]\n";
        }
        if(arg.find("anon")!=std::string::npos){
            response += "Devices:\n";
            for(Device* d: user.get_devices()){
                if(d == nullptr){
                    continue;
                }
                response += "[+] \n";
                response += "[+]  Device Name:        " + d->get_device_name() + '\n';
                response +=      "[+]  Status:             \u001b[32m" + d->get_status() + "\033[36m\n";
            }
            return Response(response.c_str(), 200);
        }
        if(response.empty())
            response="Usage: .show <devices|users|rooms>\n";

        return Response(response.c_str(), 200);
    }
    
    return Response("Unknown command, use help to get the default command list.\n", 200);
}

std::string ConsoleManagement::take_input(int user_fd, const std::string& prompt, const std::string& input_prompt) {
    
    /*
    * Function:  take_input 
    * --------------------
    * Takes the connection fd and receives input from the user.
    * user_fd = connectio file descriptor.
    * prompt (Optional) = "Login!"
    * input_prompt (Optional) = "Username: "
    *   eg. Login!
    *       Username: <input here>
    *
    *  returns: input from the user as a std:string
    */
    char* buffer = new char[DEFAULT_BUFFER_SIZE];
    memset(buffer, '\0', DEFAULT_BUFFER_SIZE);

    if(user_fd==-1){
        throw std::runtime_error("User disconnected [user_fd=-1]. Cannot take input!");
    }

    int rc = 0;
    std::string final_prompt = prompt;
    if(!prompt.empty())
        final_prompt += ((input_prompt.length() > 0) ? ("[+] " + input_prompt):" ");
    Response res = Response(final_prompt.c_str(), 200);

    bzero(buffer, DEFAULT_BUFFER_SIZE);
    res.ResponseMessage(&buffer);

    rc = send(user_fd, buffer, strlen(buffer), MSG_NOSIGNAL);
    if(rc < 0){
        fprintf(stderr, "Sending failure: %s\n", strerror(errno));
        throw std::runtime_error("take_input: Data sending error!");
    }
    
    bzero(buffer, DEFAULT_BUFFER_SIZE);
    rc = recv(user_fd, buffer, DEFAULT_BUFFER_SIZE, MSG_NOSIGNAL);
    if(rc < 0){
        fprintf(stderr, "Receiving failure: %s\n", strerror(errno));
        throw std::runtime_error("Data receiving error!");
    }
    
    buffer[strlen(buffer) - 1] = '\0';
    if (rc == 0)   
    {   
        close(user_fd);
        throw UserDisconnected("Host disconnected.", 666); 
    }
    std::string response(buffer);
    return response;
}
