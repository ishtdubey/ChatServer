/*
Author: Isht Dubey
Date: 25/04/2025
Edits: 0
code for client in a chat server application using tcp
*/

#include <iostream>
#include <cstring>  // memset()
#include <sys/socket.h> // socket(), bind(), listen(), accept(), recv(), send()
#include <netdb.h>  // getaddrinfo(), freeaddrinfo(), struct addrinfo
#include <unistd.h> // close()

int main(){
    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    int status = getaddrinfo("127.0.0.1", "3490", &hints, &res);
    if(status != 0){
        std::cerr << "getaddrinfo error: " << gai_strerror(status) << std::endl;
        return 1;
    }

    // create socket
    int socketfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if(socketfd == -1){
        std::cerr << "Socket creation failed" << std::endl;
        return 1;
    }

    // connect it to the server
    if(connect(socketfd, res->ai_addr, res->ai_addrlen) == -1){
        std::cerr << "Connection failed!" << std::endl;
        freeaddrinfo(res);  // free it if failed
        return 1;
    }

    std::cout << "Connected to server. Type messages (type 'exit' to quit):\n";

    std::string input;
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, input);

        if (input.empty()) continue;

        send(socketfd, input.c_str(), input.length(), 0);

        if (input == "exit") break;
    }

    // cleanup
    freeaddrinfo(res);
    close(socketfd);
    return 0;
}
