
// Server side C/C++ program to demonstrate Socket programming 
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h>
#include <thread>
#include <iostream>
#include <random>
#include "proto.h"
#include "user/user.h"
#include "auth.h"
#define PORT 8080

void handle_client(int socket)
{
    std::cout<<"CHUJ 1.25"<<std::endl;
    ssize_t valread;
    char buffer[4096];
    char *response;
    int response_len;
    valread = read(socket, buffer, 4096);

    std::cout<<"CHUJ 1.5"<<std::endl;
    handle_message(buffer, valread, &response, &response_len);

    send(socket, response, response_len, 0);
    close(socket);
}




void start_server()
{

    //todo to be refactored - read from config file
    User testUser{};
    testUser.login = "server";
    testUser.password = "testPassword";
    UsersHolder::instance().add(testUser);



    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                   &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address,
             sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    while(true)
    {
        if ((new_socket = accept(server_fd, (struct sockaddr *) &address,
                                 (socklen_t *) &addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        std::thread t(handle_client, new_socket);
        t.detach();
    }
}



int main(int argc, char const *argv[]) 
{
    srand(time(NULL));
    init_auth("config.txt");

    start_server();
} 