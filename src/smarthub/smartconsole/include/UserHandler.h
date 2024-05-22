#pragma once
#include <iostream>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include "User.h"
#include "Logger.h"

class UserHandler {

private:

  int connfd = -1;
  sockaddr* clientaddr;
  socklen_t* addrlen;
  User* user = nullptr;
  Logger* logger = nullptr;
public:
  UserHandler();
  ~UserHandler();
  UserHandler(int connfd, sockaddr* client_addr, socklen_t* addr_len);
  UserHandler(User** user);
  void run();

};