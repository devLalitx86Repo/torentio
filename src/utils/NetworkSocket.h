#include<bits/stdc++.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<netinet/in.h>

using namespace std;

class NetworkSocket{

public:
    string ip;
    int port;
    int socket_fd;
    struct sockaddr_in address;
    int addrlen;
    int opt = 1;

    NetworkSocket(string ip, string port){
        this->ip = ip;
        this->port = stoi(port);
        this->addrlen = sizeof(address);
    }

    NetworkSocket(){
        this->ip = "";
        this->port = 0;
    }
    

    void setup_listen(){
        if ((this->socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
            perror("socket failed");
            exit(EXIT_FAILURE);
        }
        
        if (setsockopt(this->socket_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
            perror("setsockopt");
            exit(EXIT_FAILURE);
        }

        this->address.sin_family = AF_INET;
        this->address.sin_addr.s_addr = inet_addr(this->ip.c_str());
        this->address.sin_port = htons(this->port);

        //bind socket to port
        if (bind(this->socket_fd, (struct sockaddr *)&this->address, sizeof(this->address))<0) {
            perror("bind failed");
            exit(EXIT_FAILURE);
        }

        //listen for connections
        if (listen(this->socket_fd, 3) < 0) {
            perror("listen");
            exit(EXIT_FAILURE);
        }

    }


};