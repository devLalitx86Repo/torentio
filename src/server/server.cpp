#include "../utils/global.h"

// username -> password
unordered_map<string,string> user_db; 
// username -> login_status
unordered_map<string,bool> login_db;
// username -> socket <ip>:<port>
unordered_map<string,string> user_socket_db;

// group_name -> owner name
unordered_map<string,string> group_db;
// group_name -> set of users
unordered_map<string,set<string>> group_user_db;
// group_name -> set of requests
unordered_map<string,set<string>> group_request_db;

//

vector<string> split(string str, char delimiter){
    vector<string> internal;
    stringstream ss(str);
    string tok;
 
    while(getline(ss, tok, delimiter)){
        internal.push_back(tok);
    }
 
    return internal;
}

vector<string> process_command(string command){
    vector<string> cmd_args = split(command, ' ');
    return cmd_args;
}

string exec_create_user(string username,string password){
    if(user_db.find(username) != user_db.end()){
        return "ERR: User already exists";
    }else{
        user_db[username] = password;
        return "User created successfully";
    }
}

string exec_login_user(string username,string password,string &admin_id){
    if(user_db.find(username) == user_db.end()){
        return "ERR: User doesn't exist";
    }else if(login_db[username]){
        return "ERR: User already logged in";
    }else{
        if(user_db[username] == password){
            admin_id = username;
            login_db[username] = true;
            return "Login Successful";
        }else{
            return "ERR: Wrong password";
        }
    }
}

string exec_create_group(string group_name, string& admin_id){
    if(group_db.find(group_name) != group_db.end()){
        return "ERR: Group already exists";
    }else{
        group_db[group_name] = admin_id;
        group_user_db[group_name].insert(admin_id);
        return "Group created successfully";
    }
}

string exec_list_groups(){
    string response = "Current Groups:\n";
    for(auto it = group_db.begin(); it != group_db.end(); it++){
        response += it->first + "\n";
    }
    return response;
}

string exec_join_group(string group_name,string user_id){
    if(group_db.find(group_name) == group_db.end()){
        return "ERR: Group doesn't exist";
    }else{
        // if already a user
        if(group_user_db[group_name].find(user_id) != group_user_db[group_name].end()){
            return "ERR: User already in group";
        }
        group_request_db[group_name].insert(user_id);
        return "Request sent to admin";        
    }
}

string exec_list_requests(string group_id, string user_id){
    if(group_db.find(group_id) == group_db.end()){
        return "ERR: Group doesn't exist";
    }else{
        if(group_db[group_id] != user_id){
            return "ERR: You are not the admin";
        }
        string response = "Current Requests:\n";
        for(auto it = group_request_db[group_id].begin(); it != group_request_db[group_id].end(); it++){
            response += *it + "\n";
        }
        return response;
    }
}

string exec_accept_request(string group_id,string req_user_id ,string user_id){
    if(group_db.find(group_id) == group_db.end()){
        return "ERR: Group doesn't exist";
    }else{
        if(group_db[group_id] != user_id){
            return "ERR: You are not the admin";
        }
        if(group_request_db[group_id].find(req_user_id) == group_request_db[group_id].end()){
            return "ERR: Request doesn't exist";
        }
        group_request_db[group_id].erase(req_user_id);
        group_user_db[group_id].insert(req_user_id);
        return "Request accepted";
    }
}

string exec_leave_group(string group_id, string user_id){
    if(group_db.find(group_id) == group_db.end()){
        return "ERR: Group doesn't exist";
    }else{
        if(group_user_db[group_id].find(user_id) == group_user_db[group_id].end()){
            return "ERR: User not in group";
        }
        group_user_db[group_id].erase(user_id);
        return "User left group";
    }
}

string exec_upload_file(string file_path, string group_id, string user_id, int client_socket){
    if(group_db.find(group_id) == group_db.end()){
        return "Err: Group doesn't exist";
    }
    if(group_user_db[group_id].find(user_id) == group_user_db[group_id].end()){
        return "Err: User not in group";
    }
    string file_name = split(file_path, '/').back();
    string init_res = "File upload initiated";
    send(client_socket, init_res.c_str(), init_res.length(), 0);
    char buffer[1024] = {0};
    int read_size = recv(client_socket, buffer, 1024, 0);
    if (read_size == 0)
    {
        cout << "Client got disconnected" << endl;
        return "ERR: Client got disconnected";
    }    
    cout<<"File Upload Info: "<<buffer<<endl;   
    return "File uploaded successfully";
}
/*
This method handle these client commands:
1. Start Share : share
2. 
*/
void server_routine(int client_socket){
    int read_size;
    string admin_id = "";
    while (true)
    {
        char buffer[1024] = {0};
        read_size = recv(client_socket, buffer, 1024, 0);
        if (read_size == 0)
        {
            cout << "Client got disconnected" << endl;
            // clogin_status[admin_id] = false;
            break;
        }

        // didn't consider read_size < 0 can add here #TODO
        
        string command = buffer;
        vector<string> cmd_args = process_command(command);
        command = cmd_args[0];

        if (command == "create_user")
        {   
            cout<<" REQ: Create User!"<<endl;
            if(cmd_args.size() != 3){
                string response = "ERR: Invalid Command";
                send(client_socket, response.c_str(), response.length(), 0);
                continue;
            }
            string response = exec_create_user(cmd_args[1], cmd_args[2]);             
            send(client_socket, response.c_str(), response.length(), 0);
        }
        else if(command == "login"){
            cout<<" REQ: Login User!"<<endl;
            if(cmd_args.size() != 3){
                string response = "ERR: Invalid Command";
                send(client_socket, response.c_str(), response.length(), 0);
                continue;
            }
            string response = exec_login_user(cmd_args[1], cmd_args[2], admin_id);
            send(client_socket, response.c_str(), response.length(), 0);

            if(response == "Login Successful"){
                 //recieve response from client
                char buffer[1024] = {0};
                read_size = recv(client_socket, buffer, 1024, 0);
                user_socket_db[admin_id] = buffer;
            } 
        }        
        else if(command == "create_group"){
            cout<<" REQ: Create Group!"<<endl;
            if(cmd_args.size() != 2){
                string response = "ERR: Invalid Command";
                send(client_socket, response.c_str(), response.length(), 0);
                continue;
            }
            string response = exec_create_group(cmd_args[1], admin_id);
            send(client_socket, response.c_str(), response.length(), 0);
        }
        else if(command == "join_group"){
            cout<<" REQ: Join Group!"<<endl;
            if(cmd_args.size() != 2){
                string response = "ERR: Invalid Command";
                send(client_socket, response.c_str(), response.length(), 0);
                continue;
            }
            string response = exec_join_group(cmd_args[1], admin_id);
            send(client_socket, response.c_str(), response.length(), 0);
        }
        else if(command == "list_requests"){
            cout << " REQ: List Requests!" << endl;
            if(cmd_args.size() != 2){
                string response = "ERR: Invalid Command";
                send(client_socket, response.c_str(), response.length(), 0);
                continue;
            }
            string response = exec_list_requests(cmd_args[1],admin_id);
            send(client_socket, response.c_str(), response.length(), 0);
        }
        else if(command == "list_groups"){
            cout<< " REQ: List Groups!"<<endl;
            if(cmd_args.size() != 1){
                string response = "ERR: Invalid Command";
                send(client_socket, response.c_str(), response.length(), 0);
                continue;
            }
            string response = exec_list_groups();
            send(client_socket, response.c_str(), response.length(), 0);
        }
        else if(command == "accept_request"){
            cout<<" REQ: Accept Request!"<<endl;
            if(cmd_args.size() != 3){
                string response = "ERR: Invalid Command";
                send(client_socket, response.c_str(), response.length(), 0);
                continue;
            }
            string response = exec_accept_request(cmd_args[1], cmd_args[2], admin_id);
            send(client_socket, response.c_str(), response.length(), 0);
        }
        else if(command == "leave_group"){
            cout << " REQ: Leave Group!" << endl;
            if(cmd_args.size() != 2){
                string response = "ERR: Invalid Command";
                send(client_socket, response.c_str(), response.length(), 0);
                continue;
            }
            string response = exec_leave_group(cmd_args[1], admin_id);
            send(client_socket, response.c_str(), response.length(), 0);
        }
        // else if(command == "log_out"){

        // }
        else if(command == "upload_file"){
            cout << " REQ: Upload File!" << endl;
            if(cmd_args.size() != 3){
                string response = "ERR: Invalid Command";
                send(client_socket, response.c_str(), response.length(), 0);
                continue;
            }
            string response = exec_upload_file(cmd_args[1], cmd_args[2], admin_id, client_socket);  
            send(client_socket, response.c_str(), response.length(), 0);
        }
        // else if(command == "download_file"){
        // }
        else{
            string response = "ERR: Invalid Command not recognized";
            send(client_socket, response.c_str(), response.length(), 0);
        }  

    }
}


int main(int argv, char** argc){    
    
    //input format: <server_ip>:<server_port>
    string server1_info = argc[1];
    string server2_info = argc[2];
    vector<string> server1_sinfo = split(server1_info, ':');
    vector<string> server2_sinfo = split(server2_info, ':');

    //There will be two servers
    NetworkSocket server1(server1_sinfo[0],server1_sinfo[1]), server2(server2_sinfo[0],server2_sinfo[1]);
    
    //setup listen
    server1.setup_listen();
    vector<thread> client_threads;
    while(true){
        //accept connection
        int new_socket = accept(server1.socket_fd, (struct sockaddr *)&server1.address, (socklen_t*)&server1.addrlen);
        if (new_socket<0) {
            perror("ERR: Can't accept connection...");
            exit(EXIT_FAILURE);
        }
        cout<<" LOG: New Client Connected! "<<endl;
        // start new client thread
        thread new_client_thread(server_routine, new_socket);
        client_threads.push_back(move(new_client_thread));
    }

    return 0;
}
