#pragma once
#include <iostream>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include "UserHandler.h"
#include "Logger.h"


class Server
{
    
private:
    /* Define private attributes */
    int m_socket;
    sockaddr_in m_server_addr;
    const unsigned int m_port = 12341;

    /* Define private methods */
    Server();
    static Server* ServerInstance;
    static Logger* logger;

public:

    /* Define public methods */
    Server(Server &other) = delete;
    void operator=(const Server& other) = delete;
    static Server* GetServerInstance();
    static void destroyInstance();
    int Start();
};

