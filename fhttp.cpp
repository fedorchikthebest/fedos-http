#include"fhttp.h"
#include<netinet/in.h> 
#include<sys/socket.h> 
#include<unistd.h> 
#include<queue>
#include<sstream>
#include<iostream>


std::string parse_url(std::string request){
    int counter = 0;
    std::stringstream ans;
    for (auto i:request){
        if (counter == 1 && i != ' '){
            ans << i;
        }
        if(i == ' '){
            counter += 1;
            if (counter == 2){
                break;
            }
        }
    }
    return ans.str().c_str();
}


void fhttp::Server::run(int port){
    // creating socket 
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0); 
    int buffer_size;
    
    std::string url;
    std::string response;

    // specifying the address 
    sockaddr_in serverAddress; 
    serverAddress.sin_family = AF_INET; 
    serverAddress.sin_port = htons(port); 
    serverAddress.sin_addr.s_addr = INADDR_ANY; 

    // binding socket. 
    bind(serverSocket, (struct sockaddr*)&serverAddress, 
        sizeof(serverAddress)); 

    // listening to the assigned socket 
    listen(serverSocket, 5); 
    char buffer[PACKET_SIZE] = { 0 }; 
    while (true){
        int clientSocket 
            = accept(serverSocket, nullptr, nullptr); 
        buffer_size = recv(clientSocket, buffer, sizeof(buffer) / sizeof(char), 0);
        url = parse_url(buffer);
        if (responce_functions.count(url)){
            std::cout << "request to" << url << std::endl;
            response = responce_functions[url](buffer);
            send(clientSocket, response.c_str(), response.size(), 0);
        }
        else{
            std::cout << "invalid request to" << url << std::endl;
            send(clientSocket, NOT_FOUND_RESPONSE, 25, 0);
        }
        close(clientSocket);
    }
    // closing the socket. 
    close(serverSocket);
}


void fhttp::Server::request_hendler(std::string url, std::string (*hendler)(std::string)){
    responce_functions[url] = hendler;
}