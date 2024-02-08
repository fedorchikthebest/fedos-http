#include"fhttp.h"
#include<netinet/in.h> 
#include<sys/socket.h> 
#include<unistd.h> 
#include<sstream>
#include<iostream>
#include<thread>
#include<mutex>
#include<atomic>

std::mutex mutex;
std::queue<int> tasks;
std::atomic<bool> application_run = {true};

std::map<std::string, int> commands = {
    std::pair<std::string, int>("stop", 1)
};


void console(){
    std::string command;
    while (application_run.load(std::memory_order_relaxed)){
        std::cin >> command;
        switch (commands[command])
        {
        case 1:
            application_run.store(false, std::memory_order_relaxed);
            exit(0);
            break;
        
        default:
            std::cout << "command not found" << std::endl;
            break;
        }
    }
}


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
    while (application_run.load(std::memory_order_relaxed)){
        clientSocket
            = accept(serverSocket, nullptr, nullptr); 
        mutex.lock();
        tasks.push(clientSocket);
        mutex.unlock();
    }
    close(clientSocket);
    // closing the socket. 
    close(serverSocket);
    return;
}


void fhttp::Server::requests_handler_thread(){
    int clientSocket;
    char buffer[PACKET_SIZE] = { 0 }; 
    std::string url;
    std::string response;
    int buffer_size;

    while (application_run.load(std::memory_order_relaxed)){
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
    return;
}


void fhttp::Server::run(){
    std::thread requests_handler([](fhttp::Server srv){srv.requests_handler_thread();}, *this);
    std::thread requests_accept([](fhttp::Server srv){srv.requests_accept_thread();}, *this);
    std::thread console_thread(console);

    requests_accept.join();
    requests_handler.join();
    console_thread.join();
}


void fhttp::Server::request_handler(std::string url, std::string (*hendler)(std::string)){
    responce_functions[url] = hendler;
}


fhttp::Server::Server(int p){
    port = p;
}