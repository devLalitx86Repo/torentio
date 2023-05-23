#include<sys/socket.h>
#include<netinet/in.h>
#include<iostream>
#include<unistd.h>
#include<arpa/inet.h>

#define DEF_PORT 5001
using namespace std;

int main(){
    
    // 1. Create a socket
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(client_socket < 0){
        cout << "ERR: Failed to create socket" << endl;
        return -1;
    }

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(DEF_PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;

    // Check if the server is running
    int ping = inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr);
    if(ping < 0){
        cout << "ERR: Failed to ping server" << endl;
        return -1;
    }

    // 2. Connect to server
    int connect_status = connect(client_socket, (struct sockaddr*) &server_address, sizeof(server_address));
    if(connect_status < 0){
        cout << "ERR: Failed to connect to server" << endl;
        return -1;
    }   

    // 3. Send and receive data
    string message = "Hello this is a test message from client";
    int send_status = send(client_socket, message.c_str(), message.length(), 0);
    if(send_status < 0){
        cout << "ERR: Failed to send message" << endl;
        return -1;
    }

    char buffer[1024] = {0};
    int read_status = read(client_socket, buffer, 1024);
    if(read_status < 0){
        cout << "ERR: Failed to read message" << endl;
        return -1;
    }

    cout << "Server: " << buffer << endl;

    // 4. Close the socket
    close(client_socket);

    return 0;

}