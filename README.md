# ChatServer - Multithreaded TCP Chat Server in C++

A lightweight TCP-based multithreaded chat server written in modern C++. This project demonstrates how to manage multiple client connections concurrently
using **std::thread** and **std::mutex**, with proper synchronization and graceful shutdown.


## Features

- Handles multiple clients concurrently
- Thread-safe communication using **std::mutex**
- Built using C++11 threads and POSIX sockets
- Real-time messaging between clients
- Graceful server and client shutdown logic


## How to Compile and Run
### Server
g++ server.cpp -o server -pthread
./server

### Client
g++ client.cpp -o client
./client
Open multiple terminal windows to simulate multiple clients communicating with each other through the server.

## Project Structure
ChatServer/
├── client.cpp       # Client source code
├── server.cpp       # Server source code
├── .gitignore
└── README.md

## Concepts used:
Multithreading with std::thread
Mutual exclusion with std::mutex
POSIX socket programming
Client-server architecture
Graceful exit handling

## License
This project is licensed under the MIT License — feel free to use, modify, and distribute.

## Author:
Made by Isht Dubey
