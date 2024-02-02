#include <cstring> 
#include <iostream> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <unistd.h> 
  
int main() 
{ 
    // creating socket 
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0); 
  
    // specifying address 
    sockaddr_in serverAddress; 
    serverAddress.sin_family = AF_INET; 
    serverAddress.sin_port = htons(8080); 
    serverAddress.sin_addr.s_addr = INADDR_ANY; 
  
    // sending connection request 
    connect(clientSocket, (struct sockaddr*)&serverAddress, 
            sizeof(serverAddress)); 
  
    // sending data 
    char message[2049]; 
    for (int i = 0; i < 2049; i++){
        message[i] = 'f';
    }
    send(clientSocket, message, sizeof(message) / sizeof(char), 0); 
  
    // closing socket 
    close(clientSocket); 
  
    return 0; 
}