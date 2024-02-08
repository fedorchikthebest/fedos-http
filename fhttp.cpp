#include"fhttp.h"
#include<netinet/in.h> 
#include<sys/socket.h> 
#include<unistd.h> 
#include<sstream>
#include<iostream>
#include<thread>
#include<mutex>

std::mutex mutex;
std::queue<int> tasks;
bool run = true;

std::map<std::string, int> commands = {
    std::pair
};

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


void fhttp::Server::requests_accept_thread(){
    // creating socket 
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0); 
    int clientSocket;
    int status;

    // specifying the address 
    sockaddr_in serverAddress; 
    serverAddress.sin_family = AF_INET; 
    serverAddress.sin_port = htons(port); 
    serverAddress.sin_addr.s_addr = INADDR_ANY; 

    // binding socket. 
    status = bind(serverSocket, (struct sockaddr*)&serverAddress, 
        sizeof(serverAddress));

    // listening to the assigned socket 
    listen(serverSocket, 5); 
    while (run){
        clientSocket 
            = accept(serverSocket, nullptr, nullptr); 
        std::cout << 2 << std::endl;
        mutex.lock();
        tasks.push(clientSocket);
        std::cout << tasks.front() << std::endl;
        mutex.unlock();
    }
    // closing the socket. 
    close(serverSocket);
}


void fhttp::Server::requests_handler_thread(){
    int clientSocket;
    char buffer[PACKET_SIZE] = { 0 }; 
    std::string url;
    std::string response;
    int buffer_size;

    while (run){
        mutex.lock();
        if (!tasks.empty()){
            clientSocket = tasks.front();
            tasks.pop();
            mutex.unlock();
            buffer_size = recv(clientSocket, buffer, sizeof(buffer) / sizeof(char), 0);
            url = parse_url(buffer);

            if (responce_functions.count(url)){
                std::cout << "request to " << url << std::endl;
                response = responce_functions[url](buffer);
                send(clientSocket, response.c_str(), response.size(), 0);
            }
            else{
                std::cout << "invalid request to " << url << std::endl;
                send(clientSocket, NOT_FOUND_RESPONSE, 25, 0);
            }
            close(clientSocket);
            buffer[PACKET_SIZE] = { 0 };
        }
        mutex.unlock();
    }
}


void fhttp::Server::run(){
    std::thread requests_handler([](fhttp::Server srv){srv.requests_handler_thread();}, *this);
    std::thread requests_accept([](fhttp::Server srv){srv.requests_accept_thread();}, *this);

    requests_accept.join();
    requests_handler.join();

    std::string command;

    while (true){

    }
}


void fhttp::Server::request_handler(std::string url, std::string (*hendler)(std::string)){
    responce_functions[url] = hendler;
}


fhttp::Server::Server(int p){
    port = p;
}