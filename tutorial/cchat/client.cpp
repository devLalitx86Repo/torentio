#include<thread>
#include<iostream>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>

#define PORT 5002
using namespace std;

void recv_server(int server_socket){
    char buffer[1024];
    while(true){
        int bytes_received = recv(server_socket, buffer, 1024, 0);
        if(bytes_received < 1){
            cout << "Server disconnected" << endl;
            break;
        }
        cout << "Server says: " << buffer << endl;
    }
    // while(true){
    //     int client_socket = accept(server_socket, (struct sockaddr*)&server_address, (socklen_t*)&address_length);
    //     thread t(handle_client, client_socket);
    //     t.detach();
    // }
    close(server_socket);
}

void send_msg(int server_socket){
    char buffer[1024];
    while(true){
        cout << "Enter message: ";
        string message;
        getline(cin, message);
        send(server_socket, message.c_str(), message.size() + 1, 0);
    }
}

int main(){
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;

    int connection_status = connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address));
    if(connection_status == -1){
        cout << "Error connecting to socket!" << endl;
        return 1;
    }

    // char buffer[1024];
    
    // while(true){
    //     cout << "Enter message: ";
    //     string message;
    //     getline(cin, message);
    //     send(client_socket, message.c_str(), message.size() + 1, 0);
        
    //     // int bytes_received = recv(client_socket, buffer, 1024, 0);
    //     // cout << "Server says: " << buffer << endl;
    // }
    thread t1(recv_server, client_socket);
    thread t2(send_msg, client_socket);
    t1.join();
    t2.join();
    
    close(client_socket);
    return 0;
}