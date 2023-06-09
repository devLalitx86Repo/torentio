#include<thread>
#include<iostream>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>

#define PORT 5002
using namespace std;

void handle_client(int client_socket){
    char buffer[1024];
    while(true){
        int bytes_received = recv(client_socket, buffer, 1024, 0);
        if(bytes_received < 1){
            cout << "Client disconnected" << endl;
            break;
        }
        cout << "Client says: " << buffer << endl;
        send(client_socket, buffer, bytes_received + 1, 0);
    }
    close(client_socket);
}

void send_message(int client_socket){
    char buffer[1024];
    while(true){
        cout << "Enter message: ";
        string message;
        getline(cin, message);
        send(client_socket, message.c_str(), message.size() + 1, 0);
    }
}

int main(){
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;

    bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address));
    
    listen(server_socket, 5);

    int address_length = sizeof(server_address);
    int client_socket = accept(server_socket, (struct sockaddr*)&server_address, (socklen_t*)&address_length);

    thread t1(handle_client, client_socket);
    thread t2(send_message, client_socket);
    t1.join();
    t2.join();
    // while(true){
    //     int client_socket = accept(server_socket, (struct sockaddr*)&server_address, (socklen_t*)&address_length);
    //     thread t(handle_client, client_socket);
    //     t.detach();
    // }
    
    return 0;
}
