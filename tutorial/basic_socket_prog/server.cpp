#include<sys/socket.h>
#include<netinet/in.h>
#include<iostream>
#include<unistd.h>

#define DEF_PORT 5001
using namespace std;

int main(){

    // 1. Create a socket
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(server_socket < 0){
        cout << "ERR: Failed to create socket" << endl;
        return -1;
    }

    // Attach socket to port 5001 on localhost
    int opt = 1;
    int attach_status = setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
    if(attach_status < 0){
        cout << "ERR: Failed to attach socket to port" << endl;
        return -1;
    }

    // 2. Bind the socket to an IP address and port
    struct sockaddr_in server_address;

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(DEF_PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;

    int bind_status = bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address));
    if(bind_status < 0){
        cout << "ERR: Failed to bind socket to port" << endl;
        return -1;
    }

    // 3. Listen for connections
    int listen_status = listen(server_socket, 5);
    if(listen_status < 0){
        cout << "ERR: Failed to listen for connections" << endl;
        return -1;
    }

    // 4. Accept connections
    int address_len = sizeof(server_address);
    int client_socket = accept(server_socket, (struct sockaddr*) &server_address, (socklen_t*) &address_len);
    if(client_socket < 0){
        cout << "ERR: Failed to accept connection" << endl;
        return -1;
    }

    // 5. Send and receive data
    char buffer[1024] = {0};
    int read_status = read(client_socket, buffer, 1024);
    
    cout<<"Client: "<<buffer<<endl;

    string message = "Hello this is a test message from server";
    int send_status = send(client_socket, message.c_str(), message.length(), 0);
    if(send_status < 0){
        cout << "ERR: Failed to send message" << endl;
        return -1;
    }

    // 6. Close the socket
    close(server_socket);
    shutdown(server_socket, 0);
    return 0;
    
}