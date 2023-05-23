# CPP Socket Programming

## Overview and Explanation

This is a basic socket programming application in C++. It is a simple client-server application. The client sends a message to the server and the server responds with the same message. The client then prints the message to the console.

Which concludes that there are two main parts to this application:

---
### 1. Client: Steps to follow

1. Create a socket with the `socket()` system call
```cpp
int socket(int domain, int type, int protocol);
```
2. Connect the socket to the address of the server using the `connect()` system call
```cpp
int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
```
3. Send and receive data. There are a number of ways to do this, but the simplest way is to use the `read()` and `write()` system calls.
```cpp
ssize_t read(int fd, void *buf, size_t count);
ssize_t write(int fd, const void *buf, size_t count);
```
---
### 2. Server: Steps to follow

1. Create a socket with the `socket()` system call
```cpp
int socket(int domain, int type, int protocol);
```
2. Bind the socket to an address using the `bind()` system call. For a server socket on the Internet, an address consists of a port number on the host machine.
```cpp
int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
```
3. Listen for connections with the `listen()` system call
```cpp
int listen(int sockfd, int backlog);
```
4. Accept a connection with the `accept()` system call. This call typically blocks until a client connects with the server.
```cpp
int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
```
5. Send and receive data. There are a number of ways to do this, but the simplest way is to use the `read()` and `write()` system calls.
```cpp
ssize_t read(int fd, void *buf, size_t count);
ssize_t write(int fd, const void *buf, size_t count);
```

---
## How to run the application

1. Open two terminals.
2. Complile server.cpp and client.cpp using the following commands:
```bash
g++ server.cpp -o server
g++ client.cpp -o client
```
3. Run the server in one terminal using the following command:
```bash
./server
```
4. Run the client in the other terminal using the following command:
```bash
./client
```

---
## References
- [Socket Programming in C/C++](https://www.geeksforgeeks.org/socket-programming-cc/)

---
## Author Info
- Lalit Gupta