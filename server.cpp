/*
Author: Isht Dubey
Date: 25/04/2025
Edits: 0
code for server in a chat server application using tcp
*/

#include <iostream>
#include<vector>
#include <cstring>  // memset()
#include <sys/socket.h> // socket(), bind(), listen(), accept(), recv(), send()
#include <netdb.h>  // getaddrinfo(), freeaddrinfo(), struct addrinfo
#include <unistd.h> // close()
#include<thread>
#include<mutex>
#include <algorithm> 
#include <atomic>  // For atomic boolean

std::atomic<bool> server_running(true);  // global flag to control shutdown
int server_socket {-1};
std::mutex mtx;

void handle_client(int client_fd) {
    char buffer[1024];
    while (true){
        int n = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
        if (n > 0) {
            buffer[n] = '\0';  // Null-terminate received data

            std::string msg(buffer);

            // Trim newline (\n) and carriage return (\r)
            msg.erase(std::remove(msg.begin(), msg.end(), '\n'), msg.end());
            msg.erase(std::remove(msg.begin(), msg.end(), '\r'), msg.end());

            {   // braces are used here for nested scope for limiting the critical secn
                std::lock_guard<std::mutex> lock(mtx);  // Ensure clean printing
                std::cout << "Client says: " << msg << std::endl;
            }

            // Check for exit condition
            if (msg == "exit") {
                {
                  std::lock_guard<std::mutex> lock(mtx);
                  std::cout << "Client requested to close the connection.\n";
                }
                // tell main to stop
                server_running = false;
                // this will cause accept() to return with an error
                shutdown(server_socket, SHUT_RDWR);
                close(server_socket);
                break;
            }

        } 
            else if (n == 0) {
            // Client closed the connection 
            std::lock_guard<std::mutex> lock(mtx);
            std::cout << "Client disconnected gracefully.\n";
            break;
        } 
            else {
            // Error occurred during recv()
            perror("recv");
            break;
        }
    }

    close(client_fd);  // Always clean up after done
}

int main(){
    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;    // any of the IPv4 or 6
    hints.ai_socktype = SOCK_STREAM;    //TCP socket
    hints.ai_flags = AI_PASSIVE;    // automatically use my IP

    // adress info(using addrinfo, to avoid manual setup)
    int status = getaddrinfo(NULL, "3490", &hints, &res);
    if(status != 0){
        std::cerr << "getaddrinfo failed!" << gai_strerror(status) << std::endl;
        return 1;
    }

    // creating the socket
    server_socket = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if(server_socket == -1){
        std::cerr << "socket creation failed" << std::endl;
        return 1;
    }

    // binding the socket
    if(bind(server_socket, res->ai_addr, res->ai_addrlen) == -1){
        std::cerr << "Binding failed" << std::endl;
        return 1;
    }
    freeaddrinfo(res);

    // listen it now!
    if(listen(server_socket, 5) == -1){
        std::cerr << "Error listening with backlog 5" << std::endl;
        return 1;
    }

    // successful listening!
    std::cout << "Listening to port number 3490" << std::endl;
    
    int handled{}, MAX_CLIENTS{10};
    std::vector<std::thread> threads;
    // now we will deal with the client connections
    while (server_running && handled < MAX_CLIENTS) {
        int client_fd = accept(server_socket, nullptr, nullptr);
    
        if (client_fd == -1) {
            // accept was unblocked by closing server_socket
            if (!server_running) break;
            perror("accept");
            continue;
        }
    
        threads.emplace_back(handle_client, client_fd);
        handled++;
    }

   // Wait for all client threads to complete
   for (auto& t : threads) {
    if (t.joinable())
        t.join();
}
   std::cout << "Handled " << handled << " number of clients" << std::endl;

    // cleanup
    close(server_socket);
    return 0;
}