#include<thread>
#include<iostream>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>

#define PORT 5001
using namespace std;

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

    char buffer[1024];
    while(true){
        cout << "Enter message: ";
        string message;
        getline(cin, message);
        send(client_socket, message.c_str(), message.size() + 1, 0);
        int bytes_received = recv(client_socket, buffer, 1024, 0);
        cout << "Server says: " << buffer << endl;
    }
    
    close(client_socket);
    return 0;
}