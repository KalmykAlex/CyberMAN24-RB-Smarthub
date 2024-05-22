#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <pthread.h>
#include "server.h"
#include "User.h"


Server* Server::ServerInstance = nullptr;
Logger* Server::logger = nullptr;

void* thread_function(void* user) {

    User* new_login = (User*)user;
    UserHandler* uh = new UserHandler(&new_login);
    uh->run();
    delete uh;
    pthread_exit(NULL);
}

Server::Server() {

    std::cout << "Server starting on " << this->m_port << std::endl;
    logger = Logger::GetInstance();
    memset ((sockaddr*)&this->m_server_addr, 0, sizeof (this->m_server_addr));
}

int Server::Start() {
    
    try {
        this->m_socket = socket(AF_INET, SOCK_STREAM, 0);

        if(this-> m_socket == -1) {
            std::cerr << "Socket creation failed" << std::endl;
            logger->log_error("Server: Socket creation failure.");
            return EXIT_FAILURE;
        }                        
	
	int optval=1;
	if(setsockopt(this->m_socket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0){
		std::cerr << "Server: setsockopt - Error setting SO_REUSEADDR option." << std::endl;
		logger->log_error("Server: setsockopt - Error setting SO_REUSEADDR option.");
		return EXIT_FAILURE;
	}

        this->m_server_addr.sin_family = AF_INET;
        this->m_server_addr.sin_port = htons(this->m_port);
        this->m_server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

        if(bind(this->m_socket, (sockaddr*)&this->m_server_addr, sizeof(this->m_server_addr)) != 0) {
            std::cout << "Binding failed" << std::endl;
            logger->log_error("Server: binding failed.");
	    close(m_socket);
            return EXIT_FAILURE;
        }

        if ((listen (this->m_socket, 64)) != 0) {
            std::cout << "Server not listening..." << '\n';
            logger->log_error("Server: listen failed.");
            return EXIT_FAILURE;
        }

        logger->log_info("Server starting listening on " + std::to_string(this->m_port));
        while (1) {
            try
            {
                sockaddr_in clientaddr;
                socklen_t addrlen = sizeof(clientaddr);
                int connfd;
               
                if ((connfd = accept(this->m_socket,(sockaddr*)&clientaddr, &addrlen)) < 0){
                    std::cout << "Client connection ERROR." << '\n';
                }
                else{
                    
                    std::cout << "New connection received." << std::endl;
                    User* new_login = new User(connfd, (sockaddr*)&clientaddr, addrlen);
                    new_login->set_ip_address(clientaddr.sin_addr);
                   
                    pthread_t thread;
                    if(pthread_create(&thread, NULL, thread_function, (void*)new_login) != 0){
                        logger->log_error("Server: Cannot create thread for user connection.");
                        break;
                    }
                    pthread_detach(thread);
                }
            }
            catch(const std::exception& e)
            {
                std::cerr << "Connection failure: " << e.what() << '\n';
                logger->log_error("Server error: " + std::string(e.what()));
            }
        }

	close(m_port);
        return EXIT_SUCCESS;
    }catch(...) {
	close(m_port);
        std::cerr << "Error in server:)"<< std::endl;
        logger->log_error("Server: Unexpected error.");
    }
    return 0;
}

Server *Server::GetServerInstance() {
    if(ServerInstance == nullptr) {
        ServerInstance = new Server();
    }
    return ServerInstance;
}

void Server::destroyInstance() {

    if(ServerInstance != nullptr) {
        
        delete ServerInstance;
        ServerInstance = nullptr;
        return;
    }
    logger->destroyInstance();
    return;
}
