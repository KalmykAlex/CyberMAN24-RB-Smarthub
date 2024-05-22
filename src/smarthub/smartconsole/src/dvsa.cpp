#include <iostream>
#include "server.h"

int main(int argc, char const *argv[]) {
    try {
        Server* server = Server::GetServerInstance();
        server->Start();
    }catch(std::exception& ex){
        std::cerr << "Error starting server: " << ex.what() << std::endl;
	return 1;
    }
    return 0;
}
