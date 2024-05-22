#include <unistd.h>
#include <csignal>
#include <sys/wait.h>
#include "utils.h"
#include "UserHandler.h"
#include "UserDisconnected.h"
#include "ConsoleManagement.h"

bool is_windows(std::string ip){
    return false;	
    std::string command("ping -c 1 " + ip + " | grep -o -E 'ttl=[3-6][0-9]$'");
    char output[1024]={0};
    std::cout << "Ping command executed: " << command << std::endl;
    FILE* pipe = popen(command.c_str(), "r");
    if(!pipe){
        return false;
    }

    size_t bytes_read = fread(output, sizeof(char), 1024, pipe);
    
    printf("Ping command output: %s\n", output);
    fclose(pipe);
    if(bytes_read != 0){
	return false;
    }
    return false;
}

UserHandler::UserHandler() {}

UserHandler::UserHandler(User** user) {
    this->user = *user;
    logger = Logger::GetInstance();
}

UserHandler::~UserHandler() {
}

UserHandler::UserHandler(int connfd, sockaddr* client_addr, socklen_t* addr_len) {
    logger = Logger::GetInstance();
    this->connfd = connfd;
    this->clientaddr = client_addr;
    this->addrlen = addr_len;
}

void UserHandler::run() {
    char* buffer;
    try
    {
        int connfd = this->user->get_socket();
        int rc = 0;

        if(is_windows(user->get_ip_address())){
            rc = send(connfd, WELCOME_MESSAGE_WIN, strlen(WELCOME_MESSAGE_WIN), MSG_NOSIGNAL);
        } else {
            rc = send(connfd, WELCOME_MESSAGE, strlen(WELCOME_MESSAGE), MSG_NOSIGNAL);
        }
        
        if(rc < 0){

            std::string error_str("Sending failure: " + std::string(strerror(errno)));
            fprintf(stderr, "%s", error_str.c_str());
            logger->log_error(error_str);
            throw std::runtime_error("Data sending error!");
        }

        while(true) {
            int connfd = this->user->get_socket();
	    Response res;
            if(!this->user->isLoggedIn()){
                
                
                std::string username= "";
                std::string password= "";


                while(username.empty()){
                    username = ConsoleManagement::take_input(connfd, "Login name:");
                }
                    
                while (password.empty() and username != "anonymous"){
                    password = ConsoleManagement::take_input(connfd, "Password:"); 
                }
            
                res = ConsoleManagement::EvaluateLogin(username, password);

                if(res.getCode() == 200){
                    this->user->login(username, password);
                    logger->log_info("Login from: " + user->get_ip_address() + " for user " + username);
                    
                    user->set_rooms(DBManager::GetDBInstance()->MapRoomList());
                    user->set_devices(DBManager::GetDBInstance()->MapDeviceList());
                }

		buffer = new char[res.getMessageLen() + 5];
                bzero(buffer, res.getMessageLen() + 5);

            } else {

                std::string result= "";
	                
                while(result.empty()){
                    result = ConsoleManagement::take_input(connfd);
                }
                buffer = new char[result.length() + 1];
                bzero(buffer, result.length() + 1);
                memcpy(buffer, result.c_str(), result.length());

                if(std::string(buffer) == ""){
                    res = Response("", 200);
                } else if (this->user->get_username() == "anonymous") {
                    res = ConsoleManagement::EvaluateAnonCommand(buffer, *this->user);
                } else {
                    res = ConsoleManagement::EvaluateCommand(buffer, *this->user);     
                }
                logger->log_info("Evaluating cmd: " + std::string(buffer) + " for user " + this->user->get_username() + " from " + this->user->get_ip_address());
            }
 
            rc = send(connfd, res.getMsg(), res.getMessageLen(), MSG_NOSIGNAL);
            delete buffer;
	    res.clear();

            if(rc < 0) {
		    
                logger->log_error("Error sendinf data to user " + user->get_username() + " from " + user->get_ip_address());
                fprintf(stderr, "Sending failure: %s\n", strerror(errno));
                throw std::runtime_error("Data sending error!");
            }
        }
    }
    catch(UserDisconnected& ex) {
       std::cout << "UserDisconnected: " << ex.what() << std::endl;
       logger->log_info("User " + user->get_username() + " from " + user->get_ip_address() + " disconnected.");
       delete this->user;
    }
    catch(std::exception& ex) {
       std::cout << "UserHandler error: " << ex.what() << std::endl;
       logger->log_error("UserHandler error: " + std::string(ex.what()));
       close(this->user->get_socket());
       delete this->user;
    }
    catch(...){
        std::cout << "UserHandler: Random shit go..." << std::endl;
        logger->log_error("Unknow error in UserHandler");
        close(this->user->get_socket());
	delete this->user;
    }
        
}
